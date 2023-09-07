#include "pch.h"
#include "Graphics.h"

void Graphics::Init(HWND hwnd)
{
	_hwnd = hwnd;

	CreateDeviceAndSwapChain();
	OnResize();




}
void Graphics::OnResize()
{

	assert(mDevice);
	assert(mDeviceContext);
	assert(mSwapChain);


	CreateRenderTargetView();
	CreateDepthStencilView();
	SetViewport();
}


void Graphics::RenderBegin()
{

	//om 단계에서 그려질 렌더타깃을 정해주고 렌더타긱 클리어 
	//나중에 rtv 가 아니라 texture 같은 곳에도 그림을 그릴 수도 있어요.
	mDeviceContext->OMSetRenderTargets(1, mRTV.GetAddressOf(), mDSV.Get());
	
	mDeviceContext->ClearRenderTargetView(mRTV.Get(), (float*)(&GAME->GetGameDesc().clearColor));
	mDeviceContext->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	
	mDeviceContext->RSSetViewports(1, &mViewport);
}

void Graphics::RenderEnd()
{
	//스왑체인이 그려진 도화지를 제출한다.//
	HRESULT hr = mSwapChain->Present(1, 0);
	OK(hr);
}

Vec3 Graphics::FromVertexToViewPort(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P)
{
	Matrix WVP = W * V * P;

	///이거하면 w값 나누기까지 진행 
	Vec3 after_projection = Vec3::Transform(pos, WVP);

	after_projection.x = (after_projection.x + 1.0f) * (mViewport.Width / 2) + mViewport.TopLeftX;
	after_projection.y = (-after_projection.y + 1.0f) * (mViewport.Height / 2) + mViewport.TopLeftY;
	after_projection.z = after_projection.z * (mViewport.MaxDepth - mViewport.MinDepth) + mViewport.MinDepth;

	return after_projection;
}

Vec3 Graphics::FromViewportToVertex(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P)
{
	Vec3 p = pos;

	p.x = 2.f * (p.x - mViewport.TopLeftX) /mViewport.Width - 1.f;
	p.y = -2.f * (p.y - mViewport.TopLeftY) / mViewport.Height + 1.f;
	p.z = ((p.z - mViewport.MinDepth) / (mViewport.MaxDepth - mViewport.MinDepth));

	Matrix wvp = W * V * P;
	Matrix wvpInv = wvp.Invert();

	p = Vec3::Transform(p, wvpInv);
	return p;


}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = GAME->GetGameDesc().width;
		desc.BufferDesc.Height = GAME->GetGameDesc().height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd;
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		mSwapChain.GetAddressOf(),
		mDevice.GetAddressOf(),
		nullptr,
		mDeviceContext.GetAddressOf()
	);

	OK(hr);
}




void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	OK(hr);

	hr = mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRTV.GetAddressOf());
	OK(hr);
}

void Graphics::SetViewport()
{
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(GAME->GetGameDesc().width);
	mViewport.Height = static_cast<float>(GAME->GetGameDesc().height);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

void Graphics::CreateDepthStencilView()
{
	//먼저 뎁스 스탠스용 텍스쳐를 생성한다.
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = static_cast<uint32>(GAME->GetGameDesc().width);
		desc.Height = static_cast<uint32>(GAME->GetGameDesc().height);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, mDepthStencilTexture.GetAddressOf());
		CHECK(hr);
	}
	//2.뷰를 만든다.
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = DEVICE->CreateDepthStencilView(mDepthStencilTexture.Get(), &desc, mDSV.GetAddressOf());
		CHECK(hr);
	}

}
