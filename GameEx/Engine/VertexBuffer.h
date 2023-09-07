#pragma once
class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(ComPtr<ID3D11Device> device);
	~VertexBuffer();

	//템플릿 함수 
	template <typename T>
	void CreateBuffer(const vector<T>& vertices,uint32 slot =0, bool CpuWrite =false, bool GpuWrite =false);

	inline ComPtr<ID3D11Buffer> GetComPtr() { return mVB; }
	inline uint32 GetStride() { return mStride; }
	inline uint32 GetOffset() { return mOffset; }
	inline uint32 GetCount() { return mCount; }


	inline void PushData() { CONTEXT->IASetVertexBuffers(mSlot, 1, mVB.GetAddressOf(), &mStride, &mOffset); };
private:
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11Buffer> mVB;

	uint32 mStride;
	uint32 mOffset;
	uint32 mCount;
	uint32 mSlot;

	bool bCpuWrite = false;
	bool bGpuWrite = false;

};

template <typename T>
void VertexBuffer::CreateBuffer(const vector<T>& vertices, uint32 slot , bool CpuWrite , bool GpuWrite 	)
{
	mSlot = slot;
	bCpuWrite = CpuWrite;
	bGpuWrite = GpuWrite;

	mStride = sizeof(T);
	mCount = static_cast<uint32>(vertices.size());
	mOffset = 0;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	//desc.Usage = D3D11_USAGE_IMMUTABLE;
	//일단은 기본적으로 immutable 버퍼 
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = (uint32)(mStride * mCount);

	if (bCpuWrite == false && bGpuWrite == false)
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE; // CPU Read, GPU Read
	}
	else if (bCpuWrite == true && bGpuWrite == false)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU Write, GPU Read
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (bCpuWrite == false && bGpuWrite == true) // CPU Read, GPU Write
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
	}
	else
	{
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}






	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertices.data();
	//vertices 시작점

	HRESULT hr = mDevice->CreateBuffer(&desc, &data, mVB.GetAddressOf());
	OK(hr);

}

