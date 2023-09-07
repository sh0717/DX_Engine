#include "pch.h"
#include "Light.h"

Light::Light()
	:Component(eComponentType::Light)
{
	mShadowMap = make_shared<ShadowMap>();
}

Light::~Light()
{

}

void Light::Update()
{

	Vec3 lightDir = mDirLigthDesc.Direction;
	lightDir.Normalize();

	Vec3 lightPos = -2.0f * 30.f * lightDir;

	Vec3 targetPos = Vec3(0.f);
	XMVECTOR up = ::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = ::XMMatrixLookAtLH(lightPos, targetPos, up);

	// Transform bounding sphere to light space.
	XMFLOAT3 sphereCenterLS;
	::XMStoreFloat3(&sphereCenterLS, ::XMVector3TransformCoord(targetPos, V));

	// Ortho frustum in light space encloses scene.
	float l = sphereCenterLS.x - 30.f;
	float b = sphereCenterLS.y - 30.f;
	float n = sphereCenterLS.z - 30.f;
	float r = sphereCenterLS.x + 30.f;
	float t = sphereCenterLS.y + 30.f;
	float f = sphereCenterLS.z + 30.f;
	XMMATRIX P = ::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);


	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX S = V * P * T;

	::XMStoreFloat4x4(&_lightView, V);
	::XMStoreFloat4x4(&_lightProj, P);
	::XMStoreFloat4x4(&_shadowTransform, S);
}

void Light::SortGameObject()
{

}

void Light::LightSetting()
{
	D3D11_VIEWPORT viewPort = mShadowMap->GetViewPort();
	CONTEXT->RSSetViewports(1, &viewPort);
	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	CONTEXT->OMSetRenderTargets(1, renderTargets, mShadowMap->GetDepthMapDSV().Get());

	CONTEXT->ClearDepthStencilView(mShadowMap->GetDepthMapDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);



	///이제 물체들을 그려야 되는데 ... shadow 그리는 shader 를 이용해서 
}
