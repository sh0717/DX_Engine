#include "pch.h"
#include "Camera.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "OBBCollider.h"
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "ParticleSystem.h"


Matrix Camera::ViewMatrix = Matrix::Identity;



Matrix Camera::ProjectionMatrix = Matrix::Identity;



Vec3	 Camera::CameraPos = Vec3(0.f);



Camera::Camera()
	:Component(eComponentType::Camera),mFov(XM_PI/4.f),mNear(1.f),mFar(1000.f)
{
	mWidth = static_cast<float>(GAME->GetGameDesc().width);
	mHeight = static_cast<float>(GAME->GetGameDesc().height);
}



Camera::~Camera()
{

}

void Camera::Update()
{


	UpdateMatrix();

}

void Camera::UpdateMatrix()
{
	
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	mView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);


	//S_MatView = GetTransform()->GetWorldMatrix().Invert();

	if (mtype ==eProjectionType::PERSPECTIVE)
		mProj = ::XMMatrixPerspectiveFovLH(mFov,mWidth/mHeight , mNear,mFar);
	else
		mProj = ::XMMatrixOrthographicLH(mWidth, mHeight, mNear, mFar);


	mPosition = eyePosition;
}

Matrix Camera::GetViewMatrix()
{
	return ViewMatrix;
}

Matrix Camera::GetProjectionMatrix()
{
	return ProjectionMatrix;
}



Vec3 Camera::GetCameraPosition()
{
	return CameraPos;
}

void Camera::SortGameObject()
{	
	mObjectVector.clear();
	shared_ptr<Scene> scene = SCENE->GetCurrentScene();
	vector<shared_ptr<DefaultObject>>& objs = scene->GetObjects();
	
	mFrustum = BoundingFrustum(mProj);
	BoundingFrustum WorldFrustum;
	Matrix ViewToWorld = mView.Invert();
	mFrustum.Transform(WorldFrustum, (XMMATRIX)ViewToWorld);

	
	int num = 0;
	for (auto obj : objs) {

		if (IsCulled(obj->GetLayerIndex()))
			continue;

		if (obj->GetBillboardRenderer()) {
			mBillboardObjectVector.push_back(obj);
			continue;
		}

		//if this obj is particle system then adds to mParticlesystems
		if (obj->GetParticleSystem()) {
			mParticleSystems.push_back(obj);
			continue;
		}


		if (obj->GetMeshRenderer() == nullptr
			&& obj->GetModelRenderer() == nullptr
			&& obj->GetModelAnimator() == nullptr)
			continue;


		///Frustum culling 
		
		if (obj->GetCollider()!=nullptr) {
			eColliderType type = obj->GetCollider()->GetColliderType();

			if (type == eColliderType::Sphere) {
			
				shared_ptr<SphereCollider> sphere = static_pointer_cast<SphereCollider>(obj->GetCollider());
				if (WorldFrustum.Contains(sphere->GetBoundingSphere()) == false) {
					continue;
				}
			}

			else if (type == eColliderType::AABB) {
			
				shared_ptr<AABBCollider> aabb = static_pointer_cast<AABBCollider>(obj->GetCollider());
				if (WorldFrustum.Contains(aabb->GetBoundingBox()) == false) {
					continue;
				}
			
			}


			
			num++;

		}

		mObjectVector.push_back(obj);
	}
	//WCHAR text[100] = L"";
	//::wsprintf(text, L"CULLING: %d", num);
	//::SetWindowText(GAME->GetGameDesc().hWnd, text);

	/////

	
}




void Camera::RenderShadowMap(Matrix View, Matrix Proj) {
	ProjectionMatrix = Proj;
	ViewMatrix = View;
	INSTANCING->Render(mObjectVector);


}


void Camera::RenderForward()
{
	ProjectionMatrix = mProj;
	ViewMatrix = mView;
	CameraPos = mPosition;



	//빌보드를 여기서 그릴까 ?
	//mBillboardObject 에서 그려 
	{
		for (shared_ptr<DefaultObject> obj : mBillboardObjectVector) {
			obj->GetBillboardRenderer()->Render();
		}
	}


	
	//int the mObjectvector the set of obect to draw is included 
	INSTANCING->Render(mObjectVector);



	//ParticleSystemRender
	{
		for (shared_ptr<DefaultObject> obj : mParticleSystems) {
			obj->GetParticleSystem()->Render();

		}

	}
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = ::XMVectorReplicate(d);
	XMVECTOR r = ::XMLoadFloat3(&_right);
	XMVECTOR p = ::XMLoadFloat3(&_position);
	::XMStoreFloat3(&_position, XMVectorMultiplyAdd(s, r, p));
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = ::XMVectorReplicate(d);
	XMVECTOR l = ::XMLoadFloat3(&_look);
	XMVECTOR p = ::XMLoadFloat3(&_position);
	::XMStoreFloat3(&_position, ::XMVectorMultiplyAdd(s, l, p));
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = ::XMMatrixRotationAxis(::XMLoadFloat3(&_right), angle);

	::XMStoreFloat3(&_up, ::XMVector3TransformNormal(XMLoadFloat3(&_up), R));
	::XMStoreFloat3(&_look, ::XMVector3TransformNormal(XMLoadFloat3(&_look), R));
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	::XMStoreFloat3(&_right, ::XMVector3TransformNormal(::XMLoadFloat3(&_right), R));
	::XMStoreFloat3(&_up, ::XMVector3TransformNormal(::XMLoadFloat3(&_up), R));
	::XMStoreFloat3(&_look, ::XMVector3TransformNormal(::XMLoadFloat3(&_look), R));
}




