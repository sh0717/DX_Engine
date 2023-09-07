#pragma once
#include "Pass.h"
#include "Technique.h"

struct ShaderDesc
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3DX11Effect> effect;
};

class Shader
{
public:
	friend struct Pass;

public:
	Shader(wstring file);
	~Shader();

	wstring GetFile() { return _file; }
	ComPtr<ID3DX11Effect> Effect() { return _shaderDesc.effect; }

	void Draw(UINT technique, UINT pass, UINT vertexCount, UINT startVertexLocation = 0);
	void DrawAuto(UINT technique, UINT pass);
	
	void DrawIndexed(UINT technique, UINT pass, UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT technique, UINT pass, UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT technique, UINT pass, UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);

	void Dispatch(UINT technique, UINT pass, UINT x, UINT y, UINT z);

	ComPtr<ID3DX11EffectVariable> GetVariable(string name);
	ComPtr<ID3DX11EffectScalarVariable> GetScalar(string name);
	ComPtr<ID3DX11EffectVectorVariable> GetVector(string name);
	ComPtr<ID3DX11EffectMatrixVariable> GetMatrix(string name);
	ComPtr<ID3DX11EffectStringVariable> GetString(string name);
	ComPtr<ID3DX11EffectShaderResourceVariable> GetSRV(string name);
	ComPtr<ID3DX11EffectRenderTargetViewVariable> GetRTV(string name);
	ComPtr<ID3DX11EffectDepthStencilViewVariable> GetDSV(string name);
	ComPtr<ID3DX11EffectUnorderedAccessViewVariable> GetUAV(string name);
	ComPtr<ID3DX11EffectConstantBuffer> GetConstantBuffer(string name);
	ComPtr<ID3DX11EffectShaderVariable> GetShader(string name);
	ComPtr<ID3DX11EffectBlendVariable> GetBlend(string name);
	ComPtr<ID3DX11EffectDepthStencilVariable> GetDepthStencil(string name);
	ComPtr<ID3DX11EffectRasterizerVariable> GetRasterizer(string name);
	ComPtr<ID3DX11EffectSamplerVariable> GetSampler(string name);

	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
private:
	void CreateEffect();
	ComPtr<ID3D11InputLayout> CreateInputLayout(ComPtr<ID3DBlob> fxBlob, D3DX11_EFFECT_SHADER_DESC* effectVsDesc, vector<D3D11_SIGNATURE_PARAMETER_DESC>& params);

private:
	wstring _file;
	ShaderDesc _shaderDesc;
	D3DX11_EFFECT_DESC _effectDesc;
	shared_ptr<StateBlock> _initialStateBlock;
	vector<Technique> _techniques;


public:

	void PushGlobalBuffer(const Matrix& view, const Matrix& projection, const Vec3& camerapos);
	void PushDirLightBuffer(const DirectionalLight& desc);
	void PushMaterialBuffer(const MaterialDesc& desc);
	void PushTransformBuffer(const TransformDesc& world);
	void PushBoneBuffer(const BoneDesc& desc);
	void PushKeyFrameBuffer(const KeyframeDesc& desc);
	void PushInstancedTweenBuffer(const InstancedTweenDesc& desc);
	void PushManyDirLightBuffer(const ManyDirectionalLightDesc& desc);
	void PushFrustumBuffer(const FrustumDesc& desc);
	void PushParticleSystemBuffer(const ParticleSystemsDesc& desc);
	void PushShadowBuffer(const ShadowDesc& desc);
private:

	GlobalDesc mGlobalDesc;
	shared_ptr<ConstantBuffer<GlobalDesc>> mGlobalCB;
	ComPtr<ID3DX11EffectConstantBuffer> mGlobalEffectBuffer;

	//world 업데이트 
	TransformDesc _transformDesc;
	shared_ptr<ConstantBuffer<TransformDesc>> _transformBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _transformEffectBuffer;



	DirectionalLight DirLightDesc;
	shared_ptr<ConstantBuffer<DirectionalLight>> mDirLightCB;
	ComPtr<ID3DX11EffectConstantBuffer> mDirLightEffectBuffer;



	//이건  meshrenderer에서 진행한다 .
	MaterialDesc mMaterialDesc;
	shared_ptr<ConstantBuffer<MaterialDesc>> mMaterialCB;
	ComPtr<ID3DX11EffectConstantBuffer> mMaterialEffectBuffer;



	BoneDesc mBoneDesc;
	shared_ptr<ConstantBuffer<BoneDesc>> mBoneCB;
	ComPtr<ID3DX11EffectConstantBuffer> mBoneEffectBuffer;


	KeyframeDesc mKeyframeDesc;
	shared_ptr<ConstantBuffer<KeyframeDesc>> mKeyframeCB;
	ComPtr<ID3DX11EffectConstantBuffer> mKeyframeEffectBuffer;



	InstancedTweenDesc mInstancedTweenDesc;
	shared_ptr<ConstantBuffer<InstancedTweenDesc>> mInstancedTweenCB;
	ComPtr<ID3DX11EffectConstantBuffer> mInstancedTweenEffectBuffer;



	ManyDirectionalLightDesc mManyDirLightDesc;
	shared_ptr<ConstantBuffer<ManyDirectionalLightDesc>> mManyDirLightCB;
	ComPtr<ID3DX11EffectConstantBuffer> mManyDirLightEffectBuffer;


	FrustumDesc mFrustumDesc;
	shared_ptr<ConstantBuffer<FrustumDesc>> mFrustumCB;
	ComPtr<ID3DX11EffectConstantBuffer> mFrustumEffectBuffer;


	ParticleSystemsDesc mParticleSystemDesc;
	shared_ptr<ConstantBuffer<ParticleSystemsDesc>> mParticleSystemCB;
	ComPtr<ID3DX11EffectConstantBuffer> mParticleSystemBuffer;


	ShadowDesc mShadowDesc;
	shared_ptr<ConstantBuffer<ShadowDesc>> mShadowCB;
	ComPtr<ID3DX11EffectConstantBuffer> mShadowBuffer;

};

class ShaderManager
{
public:
	static ShaderDesc GetEffect(wstring fileName);

private:
	static unordered_map<wstring, ShaderDesc> shaders;
};