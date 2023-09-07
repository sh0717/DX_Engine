#pragma once

class RenderManager;


class InstancingBuffer;

class MeshRenderer: public Component 

{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void Update() override;


	void RenderInstancing(shared_ptr< InstancingBuffer>& buffer);

	void SetPass(int32 idx) { passIDX = idx; }
	int32 GetPass() { return passIDX; }

	void SetTech(int32 idx) { techIDX = idx; }
	int32 GetTech() { return techIDX; }


	InstanceID GetInstanceID();
	
	uint16 GetID() { return ID; }
	void SetID(uint16 num) { ID = num; ID_free = num + 1; }
	inline void SetDiffID() { ID = ID_free; ID_free++; }

	inline void SetMesh(shared_ptr<Mesh> mesh) { mMesh = mesh; }
	inline void SetMaterial(shared_ptr<Material> mat) { mMaterial = mat; }
	//inline void SetRenderManager(shared_ptr<RenderManager> manager) { mRenderManager = manager; }
	/*inline void SetDiffuseTexture(shared_ptr<Texture> texture) { mDiffuseTexture = texture; }
	inline void SetShader(shared_ptr<Shader> shader) { mShader = shader; }*/

	//TODO GetMesh ¸¸µé±â

	shared_ptr<Mesh> GetMesh() { return mMesh; }
private:
	int32 TextureCount;
	shared_ptr<Mesh> mMesh;
	shared_ptr<Material> mMaterial;
	
	static uint16 ID_free;

	int32 techIDX = 0;
	int32 passIDX = 0;

	uint8 ID = 0;
	//shared_ptr<RenderManager> mRenderManager;
	//shared_ptr<Texture> mDiffuseTexture;
	//shared_ptr<Shader> mShader;

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

