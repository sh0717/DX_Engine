#include "pch.h"
#include "Texture.h"
#include <filesystem>

Texture::Texture() 
	:ResourceBase(RESOURCE_TYPE::TEXTURE),mDevice(DEVICE)
{}

Texture::Texture(ComPtr<ID3D11Device> device)
	:ResourceBase(RESOURCE_TYPE::TEXTURE),mDevice(device)
{

}

Texture::~Texture()
{

}

void Texture::Load(const wstring& path)
{

	wstring ext = std::filesystem::path(path).extension();


	DirectX::TexMetadata meta;
	DirectX::ScratchImage img;
	HRESULT hr;

	if (ext == L".dds" || ext == L".DDS")
		hr = ::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &meta, img);
	else if (ext == L".tga" || ext == L".TGA")
		hr = ::LoadFromTGAFile(path.c_str(), &meta, img);
	else // png, jpg, jpeg, bmp
		hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &meta, img);
	//hr = LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &meta, img);
	OK(hr);

	hr = CreateShaderResourceView(mDevice.Get(), img.GetImages(), img.GetImageCount(), meta, mSRV.GetAddressOf());
	OK(hr);

	mSize.x = meta.width;
	mSize.y = meta.height;

}

void Texture::LoadArray(std::vector<std::wstring>& filenames)
{
	mSRV = Utils::CreateTexture2DArraySRV(filenames);
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture::GetTexture2D()
{
	ComPtr<ID3D11Texture2D> texture;
	mSRV->GetResource((ID3D11Resource**)texture.GetAddressOf());
	return texture;
}

void Texture::CreateCubeMap(const wstring& path)
{
	DirectX::TexMetadata meta;
	DirectX::ScratchImage img;
	HRESULT hr = LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &meta, img);
	OK(hr);

	hr = CreateShaderResourceView(mDevice.Get(), img.GetImages(), img.GetImageCount(), meta, mSRV.GetAddressOf());
	OK(hr);
	//D3DX11CreateShaderResourceViewFromFile(mDevice, path, 0, 0, &mSRV, 0);

	mSize.x = meta.width;
	mSize.y = meta.height;
}



