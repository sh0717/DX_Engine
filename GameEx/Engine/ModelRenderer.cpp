#include "pch.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "InstancingBuffer.h"
#include "RenderStates.h"
uint16 ModelRenderer::ID_free = 0;
ModelRenderer::ModelRenderer(shared_ptr<Shader> shader)
	:Component(eComponentType::ModelRenderer),mShader(shader)
{
	//mRenderManager = make_shared<RenderManager>();
	//mRenderManager->Init(mShader);
}

ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::Update()
{

	return; 

	if (mModel == nullptr) {
		return;
	}

	shared_ptr<Shader> myShader = mShader;

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
		mShader->DrawIndexed(0, passIDX, mesh->indexBuffer->GetCount(), 0, 0);
	
	}




}



void ModelRenderer::RenderInstancing(shared_ptr<InstancingBuffer>& buffer)
{
	assert(mModel != nullptr);
	if (mModel == nullptr) {
		return;
	}

	shared_ptr<Shader> myShader = mShader;
	if (myShader == nullptr) {
		return;
	}

	if (myShader->PrimitiveTopology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
		CONTEXT->IASetPrimitiveTopology(myShader->PrimitiveTopology);
	}


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




	//about shadow
	ShadowDesc shadow_desc;
	if (SCENE->GetCurrentScene()->GetLights().size() > 0) {
		shadow_desc.shadowtransform = SCENE->GetCurrentScene()->GetLights()[0]->GetLight()->GetShadowTransform();
	}

	myShader->PushShadowBuffer(shadow_desc);
	if (SCENE->GetCurrentScene()->GetLights().size() > 0) {
		myShader->GetSRV("ShadowMap")->SetResource(SCENE->GetCurrentScene()->GetLights()[0]->GetLight()->GetShadowMap()->GetDepthMapSRV().Get());
	}

	//////



		///States::
	float blendFactor[4] = { 0,0,0,0 };

	CONTEXT->RSSetState(RasterizerState.Get());
	CONTEXT->OMSetBlendState(BlendState.Get(), blendFactor, 0xffffffff);
	CONTEXT->OMSetDepthStencilState(DepthStencilState.Get(), StencilRef);


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


		///1.진짜 버텍스 버퍼 입장!
		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();
		

		///2. slot1 버텍스 버퍼 인스턴스용 입장
		buffer->PushData();
		mShader->DrawIndexedInstanced(0, passIDX, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}

	
	

	CONTEXT->RSSetState(0);
	CONTEXT->OMSetBlendState(0, blendFactor, 0xffffffff);
	CONTEXT->OMSetDepthStencilState(0, StencilRef);


	if (myShader->PrimitiveTopology != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
		CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

}

void ModelRenderer::SetModel(shared_ptr<Model> model)
{
	mModel = model;

	const auto& materials = mModel->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(mShader);
	}
}

InstanceID ModelRenderer::GetInstanceID()
{
	return make_tuple( ID,(uint64)mModel.get(), (uint64)mShader.get());
}
