#include "pch.h"
#include "Material.h"


Material::Material() 
	:ResourceBase(RESOURCE_TYPE::MATERIAL)
{

	mMaterial.Diffuse = Vec4(1.0f);
	mMaterial.Ambient = Vec4(0.1f,0.1f,0.1f,1.0f);
}

Material::~Material()
{

}

void Material::SetShader(shared_ptr<Shader> shader)
{
	mShader = shader;
	mDiffuseTextureBuffer = shader->GetSRV("DiffuseMap");
	mNormalTextureBuffer = shader->GetSRV("NormalMap");
	mSpecularTextureBuffer = shader->GetSRV("SpecularMap");
	mCubeTextureBuffer = shader->GetSRV("CubeMap");
	mHeightMapBuffer = shader->GetSRV("gHeightMap");


	mDiffuseTextureArrayBuffer = shader->GetSRV("DiffuseMapArray");
	mBlendMapBuffer = shader->GetSRV("BlendMap");

}

void Material::Update() 
{
	
	if (mShader == nullptr)
		return;


	//constantbuffer 로 ads 정보 넣어줘야한다.
	mShader->PushMaterialBuffer(mMaterial);

	if (mDiffusTexture) {
		mDiffuseTextureBuffer->SetResource(mDiffusTexture->GetComPtr().Get());
	}
	else {
		mDiffuseTextureBuffer->SetResource(nullptr);
	}
	if (mNormalTexture) {
		mNormalTextureBuffer->SetResource(mNormalTexture->GetComPtr().Get());
	}
	else {
		mNormalTextureBuffer->SetResource(nullptr);
	}
	if (mSpecularTexture)
		mSpecularTextureBuffer->SetResource(mSpecularTexture->GetComPtr().Get());
	else
		mSpecularTextureBuffer->SetResource(nullptr);
	if (mCubeTexture) {
		mCubeTextureBuffer->SetResource(mCubeTexture->GetSRVComPtr().Get());
	}
	else {
		mCubeTextureBuffer->SetResource(nullptr);
	}



	if (mHeightMapTexture) {
		mHeightMapBuffer->SetResource(mHeightMapTexture->GetSRVComPtr().Get());
	}
	else {
		mHeightMapBuffer->SetResource(nullptr);
	}



	if (mDiffuseTextureArray) {
		mDiffuseTextureArrayBuffer->SetResource(mDiffuseTextureArray->GetSRVComPtr().Get());
	}
	else {
		mDiffuseTextureArrayBuffer->SetResource(nullptr);
	}

	if (mBlendTexture) {
		mBlendMapBuffer->SetResource(mBlendTexture->GetSRVComPtr().Get());
	}
	else {
		mBlendMapBuffer->SetResource(nullptr);
	}

}
