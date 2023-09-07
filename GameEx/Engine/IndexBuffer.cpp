#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ComPtr<ID3D11Device> device)
	:mDevice(device)
{

}

IndexBuffer::IndexBuffer()
	:mDevice(DEVICE)
{

}

IndexBuffer::~IndexBuffer()
{

}

void IndexBuffer::CreateBuffer(const vector<uint32>& indices)
{
	mStride = sizeof(uint32);
	mCount = static_cast<uint32>(indices.size());

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = (uint32)(mStride * mCount);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data();

	HRESULT hr = mDevice->CreateBuffer(&desc, &data, mIB.GetAddressOf());
	OK(hr);
}
