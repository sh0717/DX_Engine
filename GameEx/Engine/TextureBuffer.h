#pragma once
class TextureBuffer
{
public:
	TextureBuffer(ComPtr<ID3D11Texture2D> src);
	~TextureBuffer();

public:

	void CreateBuffer();

private:
	void CreateInput(ComPtr<ID3D11Texture2D> src);
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

public:
	uint32 GetWidth() { return mWidth; }
	uint32 GetHeight() { return mHeight; }
	uint32 GetArraySize() { return mArraySize; }

	//return as ID3D11Texture2D format 
	ComPtr<ID3D11Texture2D> GetOutput() { return (ID3D11Texture2D*)mOutput.Get(); }
	//return as SRV format 
	ComPtr<ID3D11ShaderResourceView> GetOutputSRV() { return mOutputSRV; }

public:
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return mSRV; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return mUAV; }

private:
	ComPtr<ID3D11Texture2D> mInput;
	ComPtr<ID3D11ShaderResourceView> mSRV; // Input
	ComPtr<ID3D11Texture2D> mOutput;
	ComPtr<ID3D11UnorderedAccessView> mUAV; // Output

private:
	uint32 mWidth = 0;
	uint32 mHeight = 0;
	uint32 mArraySize = 0;
	DXGI_FORMAT mFormat;
	ComPtr<ID3D11ShaderResourceView> mOutputSRV;
};

