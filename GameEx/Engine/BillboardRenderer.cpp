#include "pch.h"
#include "BillboardRenderer.h"

BillboardRenderer::BillboardRenderer()
	:Component(eComponentType::BillboardRenderer)
{

}

BillboardRenderer::~BillboardRenderer()
{

}

void BillboardRenderer::Update()
{

}

void BillboardRenderer::Create(std::vector<VertexPosSize> vec)
{
	auto object = GetGameObject();

	if (mMesh == nullptr) {
		mMesh = make_shared<Mesh>();
	}


	shared_ptr<VertexBuffer> vb = make_shared<VertexBuffer>();
	vb->CreateBuffer(vec);

	mMesh->SetVB(vb);


}



void BillboardRenderer::Render()
{
	if (mMesh == nullptr || mMaterial == nullptr) {
		return;
	}

	shared_ptr<Shader> myShader = mMaterial->GetShader();
	if (myShader == nullptr) {
		return;
	}

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	mMaterial->Update();

	myShader->PushGlobalBuffer(Camera::GetViewMatrix(), Camera::GetProjectionMatrix(), Camera::GetCameraPosition());
	
	mMesh->GetVB()->PushData();
	float blendFactor[4] = { 0,0,0,0 };


	CONTEXT->RSSetState(RasterizerState.Get());
	CONTEXT->OMSetBlendState(BlendState.Get(), blendFactor, 0xffffffff);
	CONTEXT->OMSetDepthStencilState(DepthStencilState.Get(), StencilRef);

	myShader->Draw(0, 0, mMesh->GetVB()->GetCount());

	CONTEXT->RSSetState(0);
	CONTEXT->OMSetBlendState(0, blendFactor, 0xffffffff);
	CONTEXT->OMSetDepthStencilState(0, 0);


	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
