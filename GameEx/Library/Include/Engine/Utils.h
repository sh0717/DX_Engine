#pragma once
#include "LightHelper.h"
class Utils
{
public:
	static bool StartsWith(string str, string comp);
	static bool StartsWith(wstring str, wstring comp);

	static wstring ToWString(string value);
	static string ToString(wstring value);


	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);



	static ComPtr<ID3D11ShaderResourceView> CreateTexture2DArraySRV(std::vector<std::wstring>& filenames);

	static ComPtr<ID3D11ShaderResourceView> CreateRandomTexture1DSRV(ComPtr<ID3D11Device> device);
};




struct GlobalDesc {

	Matrix View = Matrix::Identity;
	Matrix Projection = Matrix::Identity;
	Matrix ViewProjection = Matrix::Identity;
	Vec3 CameraPos = Vec3(0.f, 0.f, 0.f);
	float pad;
};


struct TransformDesc {
	Matrix World = Matrix::Identity;
	Matrix TexTransform = Matrix::Identity;
};

// Bone
#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500
struct BoneDesc {

	Matrix transforms[MAX_MODEL_TRANSFORMS];

};








// Animation
struct KeyframeDesc
{
	int32 animIndex = 0;
	uint32 currFrame = 0;
	uint32 nextFrame = 0;
	float ratio = 0.f;
	float sumTime = 0.f;
	float speed = 1.f;
	Vec2 padding;
};



struct TweenDesc
{
	TweenDesc()
	{
		curr.animIndex = 0;
		next.animIndex = -1;
	}

	void ClearNextAnim()
	{
		next.animIndex = -1;
		next.currFrame = 0;
		next.nextFrame = 0;
		next.sumTime = 0;
		tweenSumTime = 0;
		tweenRatio = 0;
	}

	float tweenDuration = 1.0f;
	float tweenRatio = 0.f;
	float tweenSumTime = 0.f;
	float padding = 0.f;
	KeyframeDesc curr;
	KeyframeDesc next;
};

struct InstancedTweenDesc
{
	TweenDesc tweens[MAX_MODEL_INSTANCE];
};




////aboutLight 
#define  MAX_DIRLIGHT_NUM 10

struct ManyDirectionalLightDesc {
	int32 num_light = 0;
	Vec3 padding;
	DirectionalLight dirlights[MAX_DIRLIGHT_NUM];
};


struct ManyPointLightDesc {

	int32 num_light = 0;
	Vec3 padding;
	PointLight pointLights[MAX_DIRLIGHT_NUM];

};


struct ManySpotLightDesc {
	int32 num_light = 0;
	Vec3 padding;
	SpotLight spotlights[MAX_DIRLIGHT_NUM];
};




struct FrustumDesc {
	Vec4 planes[6];
};


struct ParticleSystemsDesc {
	float gameTime;
	float mTimeStep;
	float mAge;
	float padding;
	Vec3 EmitPosW;
	float padding_;
	Vec3 EmitDirW;
	float padd_;
};


struct ShadowDesc {
	
	Matrix shadowtransform;


};