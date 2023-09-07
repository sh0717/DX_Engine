#pragma once
#include "ResourceBase.h"

class Texture: public ResourceBase
{

public:
	Texture();
	Texture(ComPtr<ID3D11Device> device	);
	~Texture();



	ComPtr<ID3D11ShaderResourceView> GetComPtr() { return mSRV; }
	ComPtr<ID3D11ShaderResourceView> GetSRVComPtr() { return mSRV; }

	void SetSRV(ComPtr<ID3D11ShaderResourceView> srv) { mSRV = srv; }

	void Load(const wstring& path) override;
	void LoadArray(std::vector<std::wstring>& filenames);

	//ID3D11Texture2D 포인터를 반환한다. SRV->Texture
	ComPtr<ID3D11Texture2D> GetTexture2D();

	inline Vec2 GetSize() { return mSize; }


	void CreateCubeMap(const wstring& path);
private:
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11ShaderResourceView> mSRV;
	Vec2 mSize = { 0.f,0.f };

};

