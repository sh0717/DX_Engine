#include "pch.h"
#include "Shader.h"
#include "Utils.h"

Shader::Shader(wstring file) : _file(L"..\\Shaders\\" + file)
{
	_initialStateBlock = make_shared<StateBlock>();
	{
		CONTEXT->RSGetState(_initialStateBlock->RSRasterizerState.GetAddressOf());
		CONTEXT->OMGetBlendState(_initialStateBlock->OMBlendState.GetAddressOf(), _initialStateBlock->OMBlendFactor, &_initialStateBlock->OMSampleMask);
		CONTEXT->OMGetDepthStencilState(_initialStateBlock->OMDepthStencilState.GetAddressOf(), &_initialStateBlock->OMStencilRef);
	}

	CreateEffect();
}

Shader::~Shader()
{
	
}

void Shader::CreateEffect()
{
	_shaderDesc = ShaderManager::GetEffect(_file);

	_shaderDesc.effect->GetDesc(&_effectDesc);
	for (UINT t = 0; t < _effectDesc.Techniques; t++)
	{
		Technique technique;
		technique.technique = _shaderDesc.effect->GetTechniqueByIndex(t);
		technique.technique->GetDesc(&technique.desc);
		technique.name = Utils::ToWString(technique.desc.Name);

		for (UINT p = 0; p < technique.desc.Passes; p++)
		{
			Pass pass;
			pass.pass = technique.technique->GetPassByIndex(p);
			pass.pass->GetDesc(&pass.desc);
			pass.name = Utils::ToWString(pass.desc.Name);
			pass.pass->GetVertexShaderDesc(&pass.passVsDesc);
			pass.passVsDesc.pShaderVariable->GetShaderDesc(pass.passVsDesc.ShaderIndex, &pass.effectVsDesc);

			for (UINT s = 0; s < pass.effectVsDesc.NumInputSignatureEntries; s++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC desc;

				HRESULT hr = pass.passVsDesc.pShaderVariable->GetInputSignatureElementDesc(pass.passVsDesc.ShaderIndex, s, &desc);
				CHECK(hr);

				pass.signatureDescs.push_back(desc);
			}

			pass.inputLayout = CreateInputLayout(_shaderDesc.blob, &pass.effectVsDesc, pass.signatureDescs);
			pass.stateBlock = _initialStateBlock;

			technique.passes.push_back(pass);
		}

		_techniques.push_back(technique);
	}

	for (UINT i = 0; i < _effectDesc.ConstantBuffers; i++)
	{
		ID3DX11EffectConstantBuffer* iBuffer;
		iBuffer = _shaderDesc.effect->GetConstantBufferByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		iBuffer->GetDesc(&vDesc);
	}

	for (UINT i = 0; i < _effectDesc.GlobalVariables; i++)
	{
		ID3DX11EffectVariable* effectVariable;
		effectVariable = _shaderDesc.effect->GetVariableByIndex(i);

		D3DX11_EFFECT_VARIABLE_DESC vDesc;
		effectVariable->GetDesc(&vDesc);
	}
}

ComPtr<ID3D11InputLayout> Shader::CreateInputLayout(ComPtr<ID3DBlob> fxBlob, D3DX11_EFFECT_SHADER_DESC* effectVsDesc, vector<D3D11_SIGNATURE_PARAMETER_DESC>& params)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

	for (D3D11_SIGNATURE_PARAMETER_DESC& paramDesc : params)
	{
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string name = paramDesc.SemanticName;
		std::transform(name.begin(), name.end(), name.begin(), toupper);

		if (name == "POSITION")
		{
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}

		if (Utils::StartsWith(name, "INST") == true)
		{
			elementDesc.InputSlot = 1;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}

		if (Utils::StartsWith(name, "SV_") == false)
			inputLayoutDesc.push_back(elementDesc);
	}

	const void* code = effectVsDesc->pBytecode;
	UINT codeSize = effectVsDesc->BytecodeLength;

	if (inputLayoutDesc.size() > 0)
	{
		ComPtr<ID3D11InputLayout> inputLayout;

		HRESULT hr = DEVICE->CreateInputLayout
		(
			&inputLayoutDesc[0]
			, inputLayoutDesc.size()
			, code
			, codeSize
			, inputLayout.GetAddressOf()
		);

		CHECK(hr);

		return inputLayout;
	}

	return nullptr;
}

void Shader::Draw(UINT technique, UINT pass, UINT vertexCount, UINT startVertexLocation)
{
	_techniques[technique].passes[pass].Draw(vertexCount, startVertexLocation);
}

void Shader::DrawAuto(UINT technique, UINT pass)
{
	_techniques[technique].passes[pass].DrawAuto();
}

void Shader::DrawIndexed(UINT technique, UINT pass, UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	_techniques[technique].passes[pass].DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Shader::DrawInstanced(UINT technique, UINT pass, UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	_techniques[technique].passes[pass].DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}

void Shader::DrawIndexedInstanced(UINT technique, UINT pass, UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	_techniques[technique].passes[pass].DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void Shader::Dispatch(UINT technique, UINT pass, UINT x, UINT y, UINT z)
{
	_techniques[technique].passes[pass].Dispatch(x, y, z);
}

ComPtr<ID3DX11EffectVariable> Shader::GetVariable(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str());
}

ComPtr<ID3DX11EffectScalarVariable> Shader::GetScalar(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsScalar();
}

ComPtr<ID3DX11EffectVectorVariable> Shader::GetVector(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsVector();
}

ComPtr<ID3DX11EffectMatrixVariable> Shader::GetMatrix(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsMatrix();
}

ComPtr<ID3DX11EffectStringVariable> Shader::GetString(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsString();
}

ComPtr<ID3DX11EffectShaderResourceVariable> Shader::GetSRV(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsShaderResource();
}

ComPtr<ID3DX11EffectRenderTargetViewVariable> Shader::GetRTV(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsRenderTargetView();
}

ComPtr<ID3DX11EffectDepthStencilViewVariable> Shader::GetDSV(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsDepthStencilView();
}

ComPtr<ID3DX11EffectConstantBuffer> Shader::GetConstantBuffer(string name)
{
	return _shaderDesc.effect->GetConstantBufferByName(name.c_str());
}

ComPtr<ID3DX11EffectShaderVariable> Shader::GetShader(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsShader();
}

ComPtr<ID3DX11EffectBlendVariable> Shader::GetBlend(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsBlend();
}

ComPtr<ID3DX11EffectDepthStencilVariable> Shader::GetDepthStencil(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsDepthStencil();
}

ComPtr<ID3DX11EffectRasterizerVariable> Shader::GetRasterizer(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsRasterizer();
}

ComPtr<ID3DX11EffectSamplerVariable> Shader::GetSampler(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsSampler();
}

ComPtr<ID3DX11EffectUnorderedAccessViewVariable> Shader::GetUAV(string name)
{
	return _shaderDesc.effect->GetVariableByName(name.c_str())->AsUnorderedAccessView();
}

unordered_map<wstring, ShaderDesc> ShaderManager::shaders;

ShaderDesc ShaderManager::GetEffect(wstring fileName)
{
	if (shaders.count(fileName) == 0)
	{
		ComPtr<ID3DBlob> blob;
		ComPtr<ID3DBlob> error;
		INT flag = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;

		HRESULT hr = ::D3DCompileFromFile(fileName.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, NULL, "fx_5_0", flag, NULL, blob.GetAddressOf(), error.GetAddressOf());
		if (FAILED(hr))
		{
			if (error != NULL)
			{
				string str = (const char*)error->GetBufferPointer();
				MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
			}
			assert(false);
		}

		ComPtr<ID3DX11Effect> effect;
		hr = ::D3DX11CreateEffectFromMemory(blob->GetBufferPointer(), blob->GetBufferSize(), 0, DEVICE.Get(), effect.GetAddressOf());
		CHECK(hr);
		
		shaders[fileName] = ShaderDesc{blob, effect};
	}
	
	ShaderDesc desc = shaders.at(fileName);
	ComPtr<ID3DX11Effect> effect;
	desc.effect->CloneEffect(D3DX11_EFFECT_CLONE_FORCE_NONSINGLE, effect.GetAddressOf());

	return ShaderDesc{desc.blob, effect};
}




void Shader::PushGlobalBuffer(const Matrix& view, const Matrix& projection, const Vec3& camerapos)
{
	if (mGlobalEffectBuffer == nullptr) {
		mGlobalCB = make_shared<ConstantBuffer<GlobalDesc>>();
		mGlobalEffectBuffer = GetConstantBuffer("GlobalBuffer");
	}

	mGlobalDesc.View = view;
	mGlobalDesc.Projection = projection;
	mGlobalDesc.ViewProjection = view * projection;
	mGlobalDesc.CameraPos = camerapos;

	mGlobalCB->Update(mGlobalDesc);
	mGlobalEffectBuffer->SetConstantBuffer(mGlobalCB->GetComPtr().Get());

}

void Shader::PushDirLightBuffer(const DirectionalLight& desc)
{
	if (mDirLightEffectBuffer == nullptr) {
		mDirLightEffectBuffer = GetConstantBuffer("LightBuffer");
		mDirLightCB = make_shared<ConstantBuffer<DirectionalLight>>();
	}

	DirLightDesc = desc;
	mDirLightCB->Update(DirLightDesc);
	mDirLightEffectBuffer->SetConstantBuffer(mDirLightCB->GetComPtr().Get());
}

void Shader::PushMaterialBuffer(const MaterialDesc& desc)
{
	if (mMaterialEffectBuffer == nullptr) {
		mMaterialEffectBuffer = GetConstantBuffer("MaterialBuffer");
		mMaterialCB = make_shared<ConstantBuffer<MaterialDesc>>();
	}

	mMaterialDesc = desc;
	mMaterialCB->Update(mMaterialDesc);
	mMaterialEffectBuffer->SetConstantBuffer(mMaterialCB->GetComPtr().Get());
}

void Shader::PushTransformBuffer(const TransformDesc& world)
{
	if (_transformEffectBuffer == nullptr) {
		_transformBuffer = make_shared<ConstantBuffer<TransformDesc>>();
		_transformEffectBuffer = GetConstantBuffer("TransformBuffer");
	}
	_transformDesc = world;
	_transformBuffer->Update(_transformDesc);
	_transformEffectBuffer->SetConstantBuffer(_transformBuffer->GetComPtr().Get());
}

void Shader::PushBoneBuffer(const BoneDesc& desc)
{
	if (mBoneEffectBuffer == nullptr) {
		mBoneEffectBuffer = GetConstantBuffer("BoneBuffer");
		mBoneCB = make_shared<ConstantBuffer<BoneDesc>>();
	}

	mBoneDesc = desc;
	mBoneCB->Update(mBoneDesc);
	mBoneEffectBuffer->SetConstantBuffer(mBoneCB->GetComPtr().Get());
}

void Shader::PushKeyFrameBuffer(const KeyframeDesc& desc)
{
	if (mKeyframeEffectBuffer == nullptr) {
		mKeyframeEffectBuffer = GetConstantBuffer("KeyframeBuffer");
		mKeyframeCB = make_shared<ConstantBuffer<KeyframeDesc>>();
	}

	mKeyframeDesc = desc;
	mKeyframeCB->Update(mKeyframeDesc);
	mKeyframeEffectBuffer->SetConstantBuffer(mKeyframeCB->GetComPtr().Get());
}

void Shader::PushInstancedTweenBuffer(const InstancedTweenDesc& desc)
{
	if (mInstancedTweenEffectBuffer == nullptr) {
		mInstancedTweenEffectBuffer = GetConstantBuffer("TweenBuffer");
		mInstancedTweenCB = make_shared<ConstantBuffer<InstancedTweenDesc>>();
	}

	mInstancedTweenDesc = desc;
	mInstancedTweenCB->Update(mInstancedTweenDesc);
	mInstancedTweenEffectBuffer->SetConstantBuffer(mInstancedTweenCB->GetComPtr().Get());
}

void Shader::PushManyDirLightBuffer(const ManyDirectionalLightDesc& desc)
{
	if (mManyDirLightEffectBuffer == nullptr) {
		mManyDirLightEffectBuffer = GetConstantBuffer("DirLightBuffer");
		mManyDirLightCB = make_shared<ConstantBuffer<ManyDirectionalLightDesc>>();
	}

	mManyDirLightDesc = desc;
	mManyDirLightCB->Update(mManyDirLightDesc);
	mManyDirLightEffectBuffer->SetConstantBuffer(mManyDirLightCB->GetComPtr().Get());
}

void Shader::PushFrustumBuffer(const FrustumDesc& desc)
{
	if (mFrustumEffectBuffer == nullptr) {
		mFrustumEffectBuffer = GetConstantBuffer("FrustumBuffer");
		mFrustumCB = make_shared<ConstantBuffer<FrustumDesc>>();
	}

	mFrustumDesc = desc;
	mFrustumCB->Update(mFrustumDesc);
	mFrustumEffectBuffer->SetConstantBuffer(mFrustumCB->GetComPtr().Get());
}

void Shader::PushParticleSystemBuffer(const ParticleSystemsDesc& desc)
{
	if (mParticleSystemBuffer == nullptr) {
		mParticleSystemBuffer = GetConstantBuffer("ParticleSystemBuffer");
		mParticleSystemCB = make_shared<ConstantBuffer<ParticleSystemsDesc>>();
	}

	mParticleSystemDesc = desc;
	mParticleSystemCB->Update(mParticleSystemDesc);
	mParticleSystemBuffer->SetConstantBuffer(mParticleSystemCB->GetComPtr().Get());
}

void Shader::PushShadowBuffer(const ShadowDesc& desc)
{

	if (mShadowBuffer == nullptr) {
		mShadowBuffer = GetConstantBuffer("ShadowTransform");
		mShadowCB = make_shared<ConstantBuffer<ShadowDesc>>();
	}

	mShadowDesc = desc;
	mShadowCB->Update(mShadowDesc);
	mShadowBuffer->SetConstantBuffer(mShadowCB->GetComPtr().Get());
}
