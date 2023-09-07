#pragma once

class Graphics
{
	DECLARE_SINGLE(Graphics);

public:
	void Init(HWND hwnd);
	void OnResize();


	void RenderBegin();
	void RenderEnd();
	D3D11_VIEWPORT GetViewPort() { return mViewport; }


	ComPtr<ID3D11Device> GetDevice() { return mDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return mDeviceContext; }

	//�־��� pos�� viewport���� ��ǥ�� �ٲ��ش�. 
	Vec3 FromVertexToViewPort(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);
	
	//viewport �� �־��� ���� ��ǥ�� ���� vertexbuffer �� ��ǥ�� �ٲ��ش� .
	Vec3 FromViewportToVertex(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);



private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

	void CreateDepthStencilView();


private:
	HWND _hwnd = {};

	// Device & SwapChain
	ComPtr<ID3D11Device> mDevice = nullptr;
	ComPtr<ID3D11DeviceContext> mDeviceContext = nullptr;
	ComPtr<IDXGISwapChain> mSwapChain = nullptr;

	// RTV
	ComPtr<ID3D11RenderTargetView> mRTV;


	//���� ���Ľ� ��
	ComPtr<ID3D11Texture2D> mDepthStencilTexture;
	ComPtr<ID3D11DepthStencilView> mDSV;


	// Misc
	D3D11_VIEWPORT mViewport = { 0 };
};

