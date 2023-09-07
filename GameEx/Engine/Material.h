#pragma once

class RenderManager;


class Material:public ResourceBase
{
public:


	Material();
	 virtual ~Material();

	 void Update();



	 void SetShader(shared_ptr<Shader> shader) ;
	 void SetDiffuseTexture(shared_ptr<Texture> textutre) { mDiffusTexture = textutre; }
	 void SetNormalTexture(shared_ptr<Texture> textutre) { mNormalTexture = textutre; };
	 void SetSpecularTexture(shared_ptr<Texture> textutre) { mSpecularTexture = textutre; };
	 void SetCubeTexture(shared_ptr<Texture> textutre) { mCubeTexture = textutre; };
	 void SetHeightMapTexture(shared_ptr<Texture> texture) { mHeightMapTexture = texture; }
	 void SetBlendMapTexture(shared_ptr<Texture> texture) { mBlendTexture = texture; }
	 void SetDiffuseTextureArray(shared_ptr<Texture> texture) { mDiffuseTextureArray = texture; }
	 
	 void SetMaterialDesc(const MaterialDesc& mat) { mMaterial = mat; }

	

	 //shader 를 꼭 붙여줘야 텍스텨를 붙일 수 있다카이
	 shared_ptr<Shader> GetShader() { return mShader; }

	 MaterialDesc& GetMaterialDesc() { return mMaterial; }
	 shared_ptr<Texture> GetDiffuseMap() { return mDiffusTexture; }
	 shared_ptr<Texture> GetNormalMap() { return mNormalTexture; }
	 shared_ptr<Texture> GetSpecularMap() { return mSpecularTexture; }
	 shared_ptr<Texture> GetCubeMap() { return mCubeTexture; }
	 shared_ptr<Texture> GetHeightMap() { return mHeightMapTexture; }

	 



	// shared_ptr<Material> Clone();



private:

	friend class MeshRenderer;

	shared_ptr<Shader> mShader;
	shared_ptr<Texture> mDiffusTexture;
	shared_ptr<Texture> mNormalTexture;
	shared_ptr<Texture> mSpecularTexture;

	shared_ptr<Texture> mCubeTexture;
	shared_ptr<Texture> mHeightMapTexture;

	shared_ptr<Texture> mBlendTexture;
	shared_ptr<Texture> mDiffuseTextureArray;


	MaterialDesc mMaterial;


	ComPtr<ID3DX11EffectShaderResourceVariable> mDiffuseTextureBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> mNormalTextureBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> mSpecularTextureBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> mCubeTextureBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> mHeightMapBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> mBlendMapBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> mDiffuseTextureArrayBuffer;
	


};

