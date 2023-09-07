#pragma once
#include "ShadowMap.h"
enum class eLightType {
	Direction,
	Point,
	Spot
};


class Light:public Component
{
public:
	Light();
	virtual ~Light();

	virtual void Update();


	DirectionalLight& GetLightDesc() { return mDirLigthDesc; }
	void SetLightDesc(DirectionalLight& desc) { mDirLigthDesc = desc; }


	shared_ptr<ShadowMap> GetShadowMap() { return mShadowMap; }

	/*void SetAmbient(const Color& color) { _desc.ambient = color; }
	void SetDiffuse(const Color& color) { _desc.diffuse = color; }
	void SetSpecular(const Color& color) { _desc.specular = color; }
	void SetEmissive(const Color& color) { _desc.emissive = color; }
	void SetLightDirection(Vec3 direction) { _desc.direction = direction; }*/

	void SetLightType(eLightType type) { mLightType = type; }
	eLightType GetLightType() { return mLightType; }

	Matrix GetLightView() { return _lightView; }
	Matrix GetLightProj() { return _lightProj; }
	Matrix GetShadowTransform() { return _shadowTransform; }
	
	void SortGameObject();
	void LightSetting();
	

private:

	shared_ptr<ShadowMap> mShadowMap;
	DirectionalLight mDirLigthDesc;
	PointLight mPointLightDesc;
	SpotLight mSpotLightDesc;

	eLightType mLightType=eLightType::Direction;


	Matrix _lightView;
	Matrix _lightProj;
	Matrix _shadowTransform;

	vector<shared_ptr<DefaultObject>> mObjectVector;
};

