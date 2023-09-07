#pragma once

class RenderManager
{
	//DECLARE_SINGLE(RenderManager);

public:

	void Init(shared_ptr<Shader> shader);
	void Update();


	void PushGlobalBuffer(const Matrix& view, const Matrix& projection, const Vec3& camerapos);
	void PushDirLightBuffer(const DirectionalLight& desc);
	void PushMaterialBuffer(const MaterialDesc& desc);
	void PushTransformBuffer(const TransformDesc& world);
	void PushBoneBuffer(const BoneDesc& desc);
	void PushKeyFrameBuffer(const KeyframeDesc& desc);
private:

	shared_ptr<Shader> mShader;

	//view proj 업데이트
	

};

