#include "pch.h"
#include "MeshRenderer.h"
#include "InstancingBuffer.h"
#include "Light.h"

 uint16 MeshRenderer::ID_free = 0;

MeshRenderer::MeshRenderer()
	:Component(eComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

//void MeshRenderer::Update()
//{
	//if (mMesh == nullptr || mTexture == nullptr || mShader == nullptr)
		//return;

	//auto world = GetTransform()->GetWorldMatrix();
	//_shader->GetMatrix("World")->SetMatrix((float*)&world);

	//_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	//_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	//_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());

	//// TEMP
	//Vec3 lightDir = { 0.f, 0.f, 1.f };
	//_shader->GetVector("LightDir")->SetFloatVector((float*)&lightDir);

	//uint32 stride = _mesh->GetVertexBuffer()->GetStride();
	//uint32 offset = _mesh->GetVertexBuffer()->GetOffset();

	//DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	//DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	//_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
//}


void MeshRenderer::Update()
{
	shared_ptr<Shader> myShader = mMaterial->GetShader();

	////이제는 RenderInstancing 에서 그릴겁니다.
	return;


	if (mMesh == nullptr  || mMaterial == nullptr)
		return;

	////1. diffuseTexture 넣어주기 
	//if(mMaterial->GetDiffuseMap())
	//	mMaterial->mDiffuseTextureBuffer->SetResource(mMaterial->mDiffusTexture->GetComPtr().Get());
	////mShader->GetSRV("DiffuseMap")->SetResource(nullptr);

	//if (mMaterial->GetCubeMap()) {
	//	mMaterial->mCubeTextureBuffer->SetResource(mMaterial->mCubeTexture->GetComPtr().Get());
	//}
	//

	




	mMaterial->Update();


	auto world = GetTransform()->GetWorldMatrix();
	myShader->PushTransformBuffer(TransformDesc{ world });

	///카메라 업데이트를 해주자 
	myShader->PushGlobalBuffer(Camera::GetViewMatrix(), Camera::GetProjectionMatrix(), Camera::GetCameraPosition());

	//frustum desc 업데이트 
	
	//


	mMesh->GetVB()->PushData();
	mMesh->GetIB()->PushData();


	/*uint32 stride = mMesh->GetVB()->GetStride();
	uint32 offset = mMesh->GetIB()->GetOffset();

	CONTEXT->IASetVertexBuffers(0, 1, mMesh->GetVB()->GetComPtr().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(mMesh->GetIB()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);*/

	mMaterial->GetShader()->DrawIndexed(techIDX, passIDX, mMesh->GetIB()->GetCount(), 0, 0);
}



void MeshRenderer::RenderInstancing(shared_ptr< InstancingBuffer>& buffer)
{
	if (mMesh == nullptr || mMaterial == nullptr)
		return;

	


	shared_ptr<Shader> myShader = mMaterial->GetShader();
	if (myShader == nullptr) {
		return;
	}
	if (myShader->PrimitiveTopology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
		CONTEXT->IASetPrimitiveTopology(myShader->PrimitiveTopology);
	}
	
	mMaterial->Update();

	//about Light 
	//TODO
	vector<shared_ptr<DefaultObject>>& lights = SCENE->GetCurrentScene()->GetLights();

	
	ManyDirectionalLightDesc dir_desc;
	int dir_cnt = 0;
	for (auto light : lights) {
		eLightType type = light->GetLight()->GetLightType();
		
		if (type == eLightType::Direction) {
			if (dir_cnt < 10) {
				dir_desc.dirlights[dir_cnt] = light->GetLight()->GetLightDesc();
				dir_cnt++;
			}
		}
		
	
	}
	dir_desc.num_light = dir_cnt;
	ShadowDesc shadow_desc;
	if (SCENE->GetCurrentScene()->GetLights().size() > 0) {
		shadow_desc.shadowtransform = SCENE->GetCurrentScene()->GetLights()[0]->GetLight()->GetShadowTransform();
	}
	myShader->PushManyDirLightBuffer(dir_desc);

	///about shadow
	myShader->PushShadowBuffer(shadow_desc);
	if (SCENE->GetCurrentScene()->GetLights().size() > 0) {
		myShader->GetSRV("ShadowMap")->SetResource(SCENE->GetCurrentScene()->GetLights()[0]->GetLight()->GetShadowMap()->GetDepthMapSRV().Get());
	}
	///
	///about light end

	FrustumDesc desc;
	MathHelper::ExtractFrustumPlanes(desc.planes, Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	myShader->PushFrustumBuffer(desc);


///주의사항 instancing 을 하면 이 콜이 어짜피 한번 밖에 안나오기때문에 cbuffer 로 world를 주는건 위험하다아니
	//무의미할수도 있다 .하나만나옴 무조건
	auto world = GetTransform()->GetWorldMatrix();
	myShader->PushTransformBuffer(TransformDesc{ world });

	///카메라 업데이트를 해주자 
	myShader->PushGlobalBuffer(Camera::GetViewMatrix(), Camera::GetProjectionMatrix(), Camera::GetCameraPosition());


	///진짜 버텍스버퍼 입장!
	mMesh->GetVB()->PushData();
	mMesh->GetIB()->PushData();


	///States::
	float blendFactor[4] = { 0,0,0,0 };
	//UINT a;
	//ID3D11RasterizerState* oldRasterizerState = nullptr;
	//ID3D11BlendState* oldBlendState = nullptr;
	//ID3D11DepthStencilState* oldDepthStencilState = nullptr;
	//UINT depthstencilNum;

	//CONTEXT->RSGetState(&oldRasterizerState);
	//CONTEXT->OMGetBlendState(&oldBlendState,blendFactor,&a);
	//CONTEXT->OMGetDepthStencilState(&oldDepthStencilState, &depthstencilNum);


	CONTEXT->RSSetState(RasterizerState.Get());
	CONTEXT->OMSetBlendState(BlendState.Get(), blendFactor, 0xffffffff);
	CONTEXT->OMSetDepthStencilState(DepthStencilState.Get(), StencilRef);



	//1번슬롯 버텍스 버퍼 입장!
	buffer->PushData();

	myShader->DrawIndexedInstanced(0, passIDX, mMesh->GetIB()->GetCount(), buffer->GetCount());



	//stencil restore 

	CONTEXT->RSSetState(0);
	CONTEXT->OMSetBlendState(0, blendFactor, 0xffffffff);
	CONTEXT->OMSetDepthStencilState(0, 0);



	if (myShader->PrimitiveTopology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	CONTEXT->HSSetShader(0,0,0);
	CONTEXT->DSSetShader(0, 0, 0);
}

InstanceID MeshRenderer::GetInstanceID()
{

	return make_tuple( ID,(uint64)mMesh.get(), (uint64)mMaterial.get());
}
