#include "pch.h"
#include "ModelAnimator.h"
#include "Model.h"
#include "InstancingBuffer.h"


uint16 ModelAnimator::ID_free = 0;


ModelAnimator::ModelAnimator(shared_ptr<Shader> shader)
	:Component(eComponentType::ModelAnimator),mShader(shader)
{
	//mRenderManager = make_shared<RenderManager>();
	//mRenderManager->Init(mShader);



	mTweenDesc.next.animIndex =1;
	mTweenDesc.curr.animIndex =1;
	mTweenDesc.tweenSumTime += 0;
}

ModelAnimator::~ModelAnimator()
{

}

void ModelAnimator::Update()
{
	//UpdateTweenData();
	//어짜피 이제 instancing 에서 렌더링할거다
	if (GetTransform()->mode == eTransformMode::Idle) {
		mTweenDesc.next.animIndex = 0;
		mTweenDesc.curr.speed = 0.00001;
		mTweenDesc.curr.sumTime = 0.00001;
	}
	else {
		mTweenDesc.next.animIndex = 1;
		mTweenDesc.curr.animIndex = 1;
	}

	return; 


	if (mModel == nullptr) {
		return;
	}

	if (mTexture == nullptr) {
		CreateTransformTexture();
	}

	shared_ptr<Shader> myShader = mShader;


	static KeyframeDesc descc;
	descc.animIndex += 1;
	descc.animIndex %= 3;
	descc.currFrame = 1;
	descc.speed = 1;

	


	myShader->PushKeyFrameBuffer(descc);

	mShader->GetSRV("TransformMap")->SetResource(mSRV.Get());

	BoneDesc bonedesc;

	const uint32 boneCount = mModel->GetBoneCount();

	for (uint32 i = 0; i < boneCount; ++i) {
		shared_ptr<ModelBone> bone = mModel->GetBoneByIndex(i);
		bonedesc.transforms[i] = bone->transform;
	}

	myShader->PushBoneBuffer(bonedesc);
	auto world = GetTransform()->GetWorldMatrix();
	myShader->PushTransformBuffer(TransformDesc{ world });


	//카메라 정보를 넣어주자

	myShader->PushGlobalBuffer(Camera::GetViewMatrix(), Camera::GetProjectionMatrix(), Camera::GetCameraPosition());
	//
	const auto& meshes = mModel->GetMeshes();
	//mesh vector

	for (auto& mesh : meshes) {
		if (mesh->material) {
			mesh->material->Update();
		}

		mShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		/*uint32 stride = mesh->vertexBuffer->GetStride();
		uint32 offset = mesh->vertexBuffer->GetOffset();

		CONTEXT->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		CONTEXT->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);*/

		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		mShader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
	
	}




}


//keyframe 을 업데이트하는 부분 
//this function is called at instancing manager 
void ModelAnimator::UpdateTweenData()
{
	TweenDesc& desc = mTweenDesc;

	desc.curr.sumTime += DT;
	// 현재 애니메이션
	{
		shared_ptr<ModelAnimation> currentAnim = mModel->GetAnimationByIndex(desc.curr.animIndex);
		if (currentAnim)
		{
			float timePerFrame = 1 / (currentAnim->frameRate * desc.curr.speed);
			if (desc.curr.sumTime >= timePerFrame)
			{
				desc.curr.sumTime = 0;
				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
			}

			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
		}
	}

	// 다음 애니메이션이 예약 되어 있다면
	if (desc.next.animIndex >= 0)
	{
		desc.tweenSumTime += DT;
		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;

		if (desc.tweenRatio >= 1.f)
		{
			// 애니메이션 교체 성공
			desc.curr = desc.next;
			desc.ClearNextAnim();
		}
		else
		{
			// 교체중
			shared_ptr<ModelAnimation> nextAnim = mModel->GetAnimationByIndex(desc.next.animIndex);
			desc.next.sumTime += DT;

			float timePerFrame = 1.f / (nextAnim->frameRate * desc.next.speed);

			if (desc.next.ratio >= 1.f)
			{
				desc.next.sumTime = 0;

				desc.next.currFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
				desc.next.nextFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
			}

			desc.next.ratio = desc.next.sumTime / timePerFrame;
		}
	}

}

void ModelAnimator::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{

	////참고사항 
	////tweenDesc 는 이제 그 뭐로 
	///InstancingManager 의 RenderAnimRenderer에서 진행할거다 
	if (mModel == nullptr)
		return;
	if (mTexture == nullptr)
		CreateTransformTexture();

	if (mShader == nullptr) {
		return;
	}

	if (mShader->PrimitiveTopology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
		CONTEXT->IASetPrimitiveTopology(mShader->PrimitiveTopology);
	}


	auto world = GetTransform()->GetWorldMatrix();
	mShader->PushTransformBuffer(TransformDesc{ world });

	// GlobalData
	mShader->PushGlobalBuffer(Camera::GetViewMatrix(), Camera::GetProjectionMatrix(), Camera::GetCameraPosition());


	// Light
	/*auto lightObj = SCENE->GetCurrentScene()->GetLight();
	if (lightObj)
		_shader->PushLightData(lightObj->GetLight()->GetLightDesc());*/

	// SRV를 통해 정보 전달//애니메이션의 트랜스폼들 저장 
	mShader->GetSRV("TransformMap")->SetResource(mSRV.Get());


	//about shadow
	ShadowDesc shadow_desc;
	if (SCENE->GetCurrentScene()->GetLights().size() > 0) {
		shadow_desc.shadowtransform = SCENE->GetCurrentScene()->GetLights()[0]->GetLight()->GetShadowTransform();
	}

	mShader->PushShadowBuffer(shadow_desc);
	if (SCENE->GetCurrentScene()->GetLights().size() > 0) {
		mShader->GetSRV("ShadowMap")->SetResource(SCENE->GetCurrentScene()->GetLights()[0]->GetLight()->GetShadowMap()->GetDepthMapSRV().Get());
	}

	//////


	// Bones
	BoneDesc boneDesc;

	const uint32 boneCount = mModel->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ModelBone> bone = mModel->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	mShader->PushBoneBuffer(boneDesc);


	///States::
	float blendFactor[4] = { 0,0,0,0 };
	UINT a;
	ID3D11RasterizerState* oldRasterizerState = nullptr;
	ID3D11BlendState* oldBlendState = nullptr;
	ID3D11DepthStencilState* oldDepthStencilState = nullptr;
	UINT depthstencilNum;



	//CONTEXT->RSSetState(RasterizerState.Get());
	//CONTEXT->OMSetBlendState(BlendState.Get(), blendFactor, 0xffffffff);
	//CONTEXT->OMSetDepthStencilState(DepthStencilState.Get(), StencilRef);


	const auto& meshes = mModel->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update();

		// BoneIndex
		mShader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();


		buffer->PushData();

		mShader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
	//stencil restore 

	//CONTEXT->RSSetState(0);
	//CONTEXT->OMSetBlendState(0, blendFactor, a);
	//CONTEXT->OMSetDepthStencilState(0, depthstencilNum);


	if (mShader->PrimitiveTopology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

void ModelAnimator::SetModel(shared_ptr<Model> model)
{
	mModel = model;

	const auto& materials = mModel->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(mShader);
	}
}

InstanceID ModelAnimator::GetInstanceID()
{
	return make_tuple( ID,(uint64)mModel.get(), (uint64)mShader.get());
}

void ModelAnimator::CreateTransformTexture()
{
	//애니메이션이 없으면 return;
	if (mModel->GetAnimationCount() == 0) {
		return;
	}

	//애니메이션들의 animtransform을 채워준다.
	mAnimTransforms.resize(mModel->GetAnimationCount());
	for (uint32 i = 0; i < mModel->GetAnimationCount(); ++i) {
		CreateAnimationTransform(i);
	}
	//그리고 그 mAnimTransforms 를 이용해서 texture create

	// Creature Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.ArraySize = mModel->GetAnimationCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16바이트
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		void* mallocPtr = ::malloc(pageSize * mModel->GetAnimationCount());


		for (uint32 c = 0; c < mModel->GetAnimationCount(); c++)
		{
			uint32 startOffset = c * pageSize;

			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < MAX_MODEL_KEYFRAMES; f++)
			{
				void* ptr = pageStartPtr + dataSize * f;
				::memcpy(ptr, mAnimTransforms[c].transforms[f].data(), dataSize);
			}
		}

		// 리소스 만들기
		vector<D3D11_SUBRESOURCE_DATA> subResources(mModel->GetAnimationCount());

		for (uint32 c = 0; c < mModel->GetAnimationCount(); c++)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		HRESULT hr = DEVICE->CreateTexture2D(&desc, subResources.data(), mTexture.GetAddressOf());
		CHECK(hr);

		::free(mallocPtr);
	}

	// Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = mModel->GetAnimationCount();

		HRESULT hr = DEVICE->CreateShaderResourceView(mTexture.Get(), &desc, mSRV.GetAddressOf());
		CHECK(hr);
	}


}

void ModelAnimator::CreateAnimationTransform(uint32 index)
{
	//animation 에 있는 keyframe 들에 있는 transform 은 내 조상 bones 기준
	// 따라서 root 까지 순차적으로 곱해야해 
	//mbones 에서 왼쪽이 무조건 부모다 

	vector<Matrix>  animBoneTransformsOfThisFrmae(MAX_MODEL_TRANSFORMS, Matrix::Identity);


	//애니메이션을 가져와서
	shared_ptr<ModelAnimation> animation = mModel->GetAnimationByIndex(index);

	//per frame
	for (uint32 f = 0; f < animation->frameCount; ++f) {
		//per bone
		if (f >= 500) {
			break;
		}
		for (uint32 b = 0; b < mModel->GetBoneCount(); ++b) {
			
			shared_ptr<ModelBone> bone = mModel->GetBoneByIndex(b);

			Matrix matAnimation;

			shared_ptr<ModelKeyframe> frameOfThisBone = animation->GetKeyframe(bone->name);
			if (frameOfThisBone == nullptr) {
				matAnimation = Matrix::Identity;
			}
			else {
				//data of this bone at this frame f
				ModelKeyframeData& data = frameOfThisBone->transforms[f];
				Matrix S, R, T;

				S = Matrix::CreateScale(data.scale.x, data.scale.y, data.scale.z);
				R = Matrix::CreateFromQuaternion(data.rotation);
				T = Matrix::CreateTranslation(data.translation.x, data.translation.y, data.translation.z);

				matAnimation = S * R * T;
			}

			Matrix LocalToRoot = bone->transform;
			Matrix RootToLocal = LocalToRoot.Invert();

			int32 parentIDX = bone->parentIndex;


			Matrix matParent = Matrix::Identity;
			if (parentIDX >= 0)
				matParent = animBoneTransformsOfThisFrmae[parentIDX];

			animBoneTransformsOfThisFrmae[b] = matAnimation * matParent;

			mAnimTransforms[index].transforms[f][b] =  RootToLocal*animBoneTransformsOfThisFrmae[b];
		}
	}
}
