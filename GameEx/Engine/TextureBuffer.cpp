#include "pch.h"
#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(ComPtr<ID3D11Texture2D> src)
{
	CreateInput(src);
	CreateBuffer();
}

TextureBuffer::~TextureBuffer()
{

}

void TextureBuffer::CreateBuffer()
{
	//mInput 으로부터 SRV
	CreateSRV();

	//Output을 위한 Texture2D
	CreateOutput();

	//UAV 생성 
	CreateUAV();

	//output SRV 생성 
	CreateResult();
}

void TextureBuffer::CreateInput(ComPtr<ID3D11Texture2D> src)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	mWidth = srcDesc.Width;
	mHeight = srcDesc.Height;
	mArraySize = srcDesc.ArraySize;
	mFormat = srcDesc.Format;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = mWidth;
	desc.Height = mHeight;
	desc.ArraySize = mArraySize;
	desc.Format = mFormat;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	OK(DEVICE->CreateTexture2D(&desc, NULL, mInput.GetAddressOf()));

	CONTEXT->CopyResource(mInput.Get(), src.Get());
}

void TextureBuffer::CreateSRV()
{
	D3D11_TEXTURE2D_DESC desc;
	mInput->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = mArraySize;

	OK(DEVICE->CreateShaderResourceView(mInput.Get(), &srvDesc, mSRV.GetAddressOf()));
}

void TextureBuffer::CreateOutput()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = mWidth;
	desc.Height = mHeight;
	desc.ArraySize = mArraySize;
	desc.Format = mFormat;
	//중요 부분 Bindflags 에서 bind_unordered_access 를 해줘야 uav 로 만들지 
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	OK(DEVICE->CreateTexture2D(&desc, nullptr, mOutput.GetAddressOf()));
}

void TextureBuffer::CreateUAV()
{
	D3D11_TEXTURE2D_DESC desc;
	mOutput->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = mArraySize;

	OK(DEVICE->CreateUnorderedAccessView(mOutput.Get(), &uavDesc, mUAV.GetAddressOf()));
}

void TextureBuffer::CreateResult()
{
	D3D11_TEXTURE2D_DESC desc;
	mOutput->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = mArraySize;

	CHECK(DEVICE->CreateShaderResourceView(mOutput.Get(), &srvDesc, mOutputSRV.GetAddressOf()));
}
