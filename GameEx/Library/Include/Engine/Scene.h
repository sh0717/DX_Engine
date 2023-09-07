#pragma once
#include "unordered_set"
#include "ShadowMap.h"
class Scene
{
public:
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	virtual void RenderShadowMap();
	virtual void Render();

	virtual void Add(shared_ptr<DefaultObject> object);
	virtual void Remove(shared_ptr<DefaultObject> object);


	virtual vector<shared_ptr<DefaultObject>>& GetLights() { return mLights; }
	virtual vector<shared_ptr<DefaultObject>>& GetObjects() { return mObjects; }

	


	shared_ptr<DefaultObject> GetMainCamera();
	shared_ptr<DefaultObject> GetUICamera();


	shared_ptr<DefaultObject> Pick(int32 screenX, int32 screenY);

private:
	vector<shared_ptr<DefaultObject>> mObjects;
	// Cache Camera
	vector<shared_ptr<DefaultObject>> mCameras;
	// Cache Light
	vector<shared_ptr<DefaultObject>> mLights;


	

};

