#pragma once

//light component 에다가 shadowmap 을 붙이는게 맞는거 같아
//this shadowMap have the shadow texture 
//fill this texture when drawing shadowmap in the scene 
//use the texture to make shadow in the draw call 
class ShadowMap
{
public:
	ShadowMap(uint32 width=2048, uint32 height=2048);
	~ShadowMap();

	ComPtr<ID3D11ShaderResourceView> GetDepthMapSRV();
	ComPtr<ID3D11DepthStencilView> GetDepthMapDSV();
	D3D11_VIEWPORT GetViewPort() { return _viewport; }

private:
	uint32 _width;
	uint32 _height;


	ComPtr<ID3D11ShaderResourceView> _depthMapSRV;
	ComPtr<ID3D11DepthStencilView> _depthMapDSV;

	D3D11_VIEWPORT _viewport;
};

