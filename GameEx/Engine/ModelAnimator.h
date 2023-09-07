#pragma once

//model animator 에서 keyframes 를 cbuffer 로 주기 어렵다 
//그래서 texture2d 를 만들어서 건내준다. 

//500프레임까지
struct AnimTransform
{
	// [ ][ ][ ][ ][ ][ ][ ] ... 250개
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;
	// [ ][ ][ ][ ][ ][ ][ ] ... 500 개
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;
};

class Model;
class InstancingBuffer;
class ModelAnimator:public Component
{

public:

	ModelAnimator(shared_ptr<Shader> shader);
	virtual ~ModelAnimator();

	virtual void Update();

	inline shared_ptr<Shader> GetShader() { return mShader; }
	void UpdateTweenData();
	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	void SetModel(shared_ptr<Model> model);
	inline void SetPass(uint8 pass) {
		_pass = pass;
	}
	inline void SetTech(uint8 tech) { _tech = tech; }

	InstanceID GetInstanceID();
	TweenDesc& GetTweenDesc() { return mTweenDesc; }


	uint16 GetID() { return ID; }
	void SetID(uint16 num) { ID = num; ID_free = num + 1; }
	inline void SetDiffID() { ID = ID_free; ID_free++; }
private:

	static uint16 ID_free;

	//mAnimation의 정보를  texture 로 변환
	void CreateTransformTexture();

	//model 의 index번 애니메이션의 정보로 mAnimTransforms[index] 를 채운다
	void CreateAnimationTransform(uint32 index);

private:
	shared_ptr<Shader> mShader;
	//shared_ptr<RenderManager> mRenderManager;

	uint8 _pass = 0;
	uint8 _tech = 0;

	shared_ptr<Model> mModel;


	vector<AnimTransform> mAnimTransforms;

	//텍스쳐용
	ComPtr<ID3D11Texture2D> mTexture;
	ComPtr<ID3D11ShaderResourceView> mSRV;


	TweenDesc mTweenDesc;
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

