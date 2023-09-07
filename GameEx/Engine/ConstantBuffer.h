#pragma once

template<typename T>
class ConstantBuffer
{
public:

	ConstantBuffer() 
		:mDevice(DEVICE),mDeviceContext(CONTEXT)
	{
		Create();
	}
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
		: mDevice(device), mDeviceContext(deviceContext)
	{
		Create();
	}

	~ConstantBuffer() { }

	ComPtr<ID3D11Buffer> GetComPtr() { return mCB; }

	void Create()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_Write + GPU_Read
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = mDevice->CreateBuffer(&desc, nullptr, mCB.GetAddressOf());
		OK(hr);
	}

	void Update(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		mDeviceContext->Map(mCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, &data, sizeof(data));
		mDeviceContext->Unmap(mCB.Get(), 0);
	}

private:
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11DeviceContext> mDeviceContext;
	ComPtr<ID3D11Buffer> mCB;
};
