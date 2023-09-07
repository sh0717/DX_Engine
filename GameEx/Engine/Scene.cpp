#include "pch.h"
#include "Scene.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
void Scene::Start()
{
	vector<shared_ptr<DefaultObject>> objs = mObjects;
	vector<shared_ptr<DefaultObject>> cam = mCameras;
	vector<shared_ptr<DefaultObject>> lig = mLights;


	for (shared_ptr<DefaultObject> object : objs)
	{
		object->Start();
	}



}

void Scene::Update()
{
	vector<shared_ptr<DefaultObject>> objs = mObjects;
	


	for (shared_ptr<DefaultObject> object : objs)
	{
		object->Update();
	}




	//vector<shared_ptr<DefaultObject>> temp;
	//temp.insert(temp.end(), mObjects.begin(), mObjects.end());
	//INSTANCING->Render(temp);
}

void Scene::LateUpdate()
{
	vector<shared_ptr<DefaultObject>> objs = mObjects;
	

	for (shared_ptr<DefaultObject> object : objs)
	{
		object->LateUpdate();
	}




}

void Scene::RenderShadowMap() {

	for (auto& camera : mCameras)
	{
		camera->GetCamera()->SortGameObject();
		
	}

	ID3D11RenderTargetView* oldrenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	D3D11_VIEWPORT oldViewPort;
	CONTEXT->OMGetRenderTargets(1,&oldrenderTargetView,&oldDepthStencilView);
	UINT num=1;
	CONTEXT->RSGetViewports(&num, &oldViewPort);

	//draw to shadowmap changin the rtv and dsv 


	for (shared_ptr<DefaultObject> light_object : mLights) {
		light_object->GetLight()->LightSetting();

		for (auto& camera : mCameras)
		{
			if (camera->GetCamera()->cameraType == eCameraType::Default) {
				camera->GetCamera()->RenderShadowMap(light_object->GetLight()->GetLightView(), light_object->GetLight()->GetLightProj());
			}
		}

	}







	//RTV DSV 복구
	CONTEXT->OMSetRenderTargets(1, &oldrenderTargetView, oldDepthStencilView);
	CONTEXT->RSSetViewports(1, &oldViewPort);
	

}

void Scene::Render()
{
	for (auto& camera : mCameras)
	{
		
		camera->GetCamera()->RenderForward();
	}
}

void Scene::Add(shared_ptr<DefaultObject> object)
{
	mObjects.push_back(object);

	if (object->GetCamera() != nullptr)
	{
		mCameras.push_back(object);
	}

	if (object->GetLight() != nullptr)
	{
		mLights.push_back(object);
		
	}

	
}

void Scene::Remove(shared_ptr<DefaultObject> object)
{
	
	//Remove, erase

	mObjects.erase(remove(mObjects.begin(), mObjects.end(), object), mObjects.end());
	mLights.erase(remove(mLights.begin(), mLights.end(), object), mLights.end());
	mCameras.erase(remove(mCameras.begin(), mCameras.end(), object), mCameras.end());
}

std::shared_ptr<DefaultObject> Scene::GetMainCamera()
{
	for (auto& camera : mCameras)
	{
		if (camera->GetCamera()->GetProjectionType() == eProjectionType::PERSPECTIVE)
			return camera;
	}

	return nullptr;
}

std::shared_ptr<DefaultObject> Scene::GetUICamera()
{
	for (auto& camera : mCameras)
	{
		if (camera->GetCamera()->GetProjectionType() == eProjectionType::ORTHO)
			return camera;
	}

	return nullptr;
}

std::shared_ptr<DefaultObject> Scene::Pick(int32 screenX, int32 screenY)
{
	shared_ptr<Camera> cam= mCameras[0]->GetCamera();

	float width = GRAPHICS->GetViewPort().Width;
	float height = GRAPHICS->GetViewPort().Height;


	Matrix P = cam->GetProjectionMatrix();
	Matrix V = cam->GetViewMatrix();
	Matrix Vinv = V.Invert();
	float ViewX = (2.0f * screenX / width - 1.f) / P(0,0);
	float ViewY = (-2.0f * screenY / height+ 1.f) / P(1,1);

	float minDistance = FLT_MAX;
	shared_ptr<DefaultObject> picked;


	// ViewSpace에서의 Ray 정의
	Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Vec4 rayDir = Vec4(ViewX, ViewY, 1.0f, 0.0f);

	// WorldSpace에서의 Ray 정의
	Vec3 worldRayOrigin = XMVector3TransformCoord(rayOrigin, Vinv);
	Vec3 worldRayDir = XMVector3TransformNormal(rayDir, Vinv);

	worldRayDir.Normalize();
	Ray ray = Ray(worldRayOrigin, worldRayDir);

	for (auto& object : mObjects) {
		if (object->GetCollider() == nullptr) {
			continue;
		}

		shared_ptr<SphereCollider> coll = static_pointer_cast<SphereCollider>(object->GetCollider());

		Vec3 pos = coll->GetBoundingSphere().Center;
		float rad = coll->GetBoundingSphere().Radius;

		float distanc = 0.f;
		if (object->GetCollider()->IsIntersects(ray, distanc) == false) {
			continue;
		}
		if (distanc < minDistance) {
			minDistance = distanc;
			picked = object;
		}
	}

	return picked;

}
