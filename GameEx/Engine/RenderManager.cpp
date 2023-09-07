#include "pch.h"
#include "RenderManager.h"

void RenderManager::Init(shared_ptr<Shader> shader)
{
	/*mShader = shader;

	mGlobalCB = make_shared<ConstantBuffer<GlobalDesc>>();
	mGlobalEffectBuffer = mShader->GetConstantBuffer("GlobalBuffer");


	_transformBuffer = make_shared<ConstantBuffer<TransformDesc>>();

	_transformEffectBuffer = mShader->GetConstantBuffer("TransformBuffer");

	mDirLightCB = make_shared<ConstantBuffer<DirectionalLight>>();
	mDirLightEffectBuffer = mShader->GetConstantBuffer("LightBuffer");

	mMaterialCB = make_shared<ConstantBuffer<MaterialDesc>>();
	mMaterialEffectBuffer = mShader->GetConstantBuffer("MaterialBuffer");


	mBoneCB = make_shared<ConstantBuffer<BoneDesc>>();
	mBoneEffectBuffer = mShader->GetConstantBuffer("BoneBuffer");


	mKeyframeCB = make_shared<ConstantBuffer<KeyframeDesc>>();
	mKeyframeEffectBuffer = mShader->GetConstantBuffer("KeyframeBuffer");*/

}

void RenderManager::Update()
{
	//PushGlobalBuffer(Camera::GetViewMatrix(), Camera::GetProjectionMatrix(), Camera::GetCameraPosition());
}

void RenderManager::PushGlobalBuffer(const Matrix& view, const Matrix& projection, const Vec3 & camerapos)
{

	mGlobalDesc.View = view;
	mGlobalDesc.Projection = projection;
	mGlobalDesc.ViewProjection = view * projection;
	mGlobalDesc.CameraPos = camerapos;

	mGlobalCB->Update(mGlobalDesc);
	mGlobalEffectBuffer->SetConstantBuffer(mGlobalCB->GetComPtr().Get());
}

void RenderManager::PushDirLightBuffer(const DirectionalLight& desc)
{
	DirLightDesc = desc;
	mDirLightCB->Update(DirLightDesc);
	mDirLightEffectBuffer->SetConstantBuffer(mDirLightCB->GetComPtr().Get());

}

void RenderManager::PushMaterialBuffer(const MaterialDesc& desc)
{
	mMaterialDesc = desc;
	mMaterialCB->Update(mMaterialDesc);
	mMaterialEffectBuffer->SetConstantBuffer(mMaterialCB->GetComPtr().Get());

}

void RenderManager::PushTransformBuffer(const TransformDesc& world)
{
	_transformDesc = world;
	_transformBuffer->Update(_transformDesc);
	_transformEffectBuffer->SetConstantBuffer(_transformBuffer->GetComPtr().Get());
}

void RenderManager::PushBoneBuffer(const BoneDesc& desc)
{
	mBoneDesc = desc;
	mBoneCB->Update(mBoneDesc);
	mBoneEffectBuffer->SetConstantBuffer(mBoneCB->GetComPtr().Get());

}

void RenderManager::PushKeyFrameBuffer(const KeyframeDesc& desc)
{
	mKeyframeDesc = desc;
	mKeyframeCB->Update(mKeyframeDesc);
	mKeyframeEffectBuffer->SetConstantBuffer(mKeyframeCB->GetComPtr().Get());

}
