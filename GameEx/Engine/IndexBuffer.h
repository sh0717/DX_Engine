#pragma once
class IndexBuffer
{
public:
	IndexBuffer();

	IndexBuffer(ComPtr<ID3D11Device> device);
	~IndexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() { return mIB; }
	uint32 GetStride() { return mStride; }
	uint32 GetOffset() { return mOffset; }
	uint32 GetCount() { return mCount; }

	void CreateBuffer(const vector<uint32>& indices);

	inline void PushData() {
		CONTEXT->IASetIndexBuffer(mIB.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
private:
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11Buffer> mIB;

	uint32 mStride = 0;
	uint32 mOffset = 0;
	uint32 mCount = 0;


};

