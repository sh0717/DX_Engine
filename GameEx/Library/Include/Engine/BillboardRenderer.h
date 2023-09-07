#pragma once



class BillboardRenderer:public Component
{

public:
	BillboardRenderer();
	virtual ~BillboardRenderer();

	virtual void Update() override;

	void Render();
	void Create(std::vector<VertexPosSize> vec);


	inline void SetMesh(shared_ptr<Mesh> mesh) { mMesh = mesh; }
	inline void SetMaterial(shared_ptr<Material> mat) { mMaterial = mat; }
private:

	shared_ptr<Mesh> mMesh;
	shared_ptr<Material> mMaterial;



public:
	ComPtr<ID3D11RasterizerState> GetRasterizerState() { return RasterizerState; }
	void SetRasterizerState(ComPtr<ID3D11RasterizerState> state) { RasterizerState = state; }
	ComPtr<ID3D11BlendState> GetBlendState() { return BlendState; }
	void SetBlendState(ComPtr<ID3D11BlendState> state) { BlendState = state; }
	ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return DepthStencilState; }
	void SetDepthStencilState(ComPtr<ID3D11DepthStencilState> state) { DepthStencilState = state; }
	UINT StencilRef = 0;

private:

	ComPtr<ID3D11RasterizerState> RasterizerState;
	ComPtr<ID3D11BlendState> BlendState;
	ComPtr<ID3D11DepthStencilState> DepthStencilState;

};

