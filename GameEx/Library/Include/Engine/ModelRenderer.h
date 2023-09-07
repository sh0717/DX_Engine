#pragma once


class Model;
class InstancingBuffer;
class ModelRenderer:public Component
{

public:

	ModelRenderer(shared_ptr<Shader> shader);
	virtual ~ModelRenderer();

	virtual void Update();
	void RenderInstancing(shared_ptr< InstancingBuffer>& buffer);


	void SetModel(shared_ptr<Model> model);
	inline void SetPass(uint8 pass) {
		passIDX = pass;
	}
	inline void SetTech(uint8 tech) { techIDX = tech; }

	inline uint8 GetPass() { return passIDX; }
	inline uint8 GetTech() { return techIDX; }


	InstanceID GetInstanceID();

	uint16 GetID() { return ID; }
	void SetID(uint16 num) { ID = num; ID_free = num + 1; }
	inline void SetDiffID() { ID = ID_free; ID_free++; }
private:

	static uint16 ID_free;

	shared_ptr<Shader> mShader;
	//shared_ptr<RenderManager> mRenderManager;

	uint8 passIDX = 0;
	uint8 techIDX = 0;

	shared_ptr<Model> mModel;
	uint8 ID = 0;


	//about state 

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

