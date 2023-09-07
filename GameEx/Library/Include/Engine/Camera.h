#pragma once

enum class eProjectionType {
	PERSPECTIVE,
	ORTHO
};

enum class eCameraType {

	Default,
	UI

};

class Camera:public Component
{
public:
	Camera();
	~Camera();

	virtual void Update() override;
	void SetMode(eProjectionType mode) { mtype = mode; }
	eProjectionType GetMode() { return mtype; }

	void UpdateMatrix();

	inline void SetNear(float num) { mNear = num; }
	inline void SetFar(float num) { mFar = num; }
	inline void SetFov(float num) { mFov = num; }
	inline void SetWidth(float num) { mWidth = num; }
	inline void SetHeight(float num) { mHeight = num; }


	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);



	float GetNear() { return mNear; }
	float GetFar() { return mFar; }
	float GetFov() { return mFov; }
	float GetWidth() { return mWidth; }
	float GetHeight() { return mHeight; }





	static Matrix GetViewMatrix();
	static Matrix GetProjectionMatrix();
	static Vec3 GetCameraPosition();


	inline Matrix GetMyView() { return mView; }
	inline Matrix GetMyProj() { return mProj; }
	inline Vec3 GetMyPos() { return mPosition; }

	eProjectionType GetProjectionType() { return mtype; }
	void SetProjectionType(eProjectionType type) { mtype = type; }


	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			mCullingMask |= (1 << layer);
		else
			mCullingMask &= ~(1 << layer);
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void SetCullingMask(uint32 mask) { mCullingMask = mask; }
	bool IsCulled(uint8 layer) { return (mCullingMask & (1 << layer)) != 0; }


	//scene 의 object 들을 체크하면서 masking 을 통해 렌더링 할것만 추려낸다. 
	void SortGameObject();

	//shadowmap 에 그리는 작업 
	void RenderShadowMap(Matrix View, Matrix Proj);

	//추려낸 objects 를 instancing rendering 진행
	void RenderForward();
	eCameraType cameraType = eCameraType::Default;
private:
	eProjectionType mtype = eProjectionType::PERSPECTIVE;

	float mNear;
	float mFar;
	float mFov;
	float mWidth;
	float mHeight;

	XMFLOAT3 _position = { 0, 0, -10.f };
	XMFLOAT3 _right = { 1, 0, 0 };
	XMFLOAT3 _up = { 0, 1, 0 };
	XMFLOAT3 _look = { 0, 0, 1 };


	static Matrix ViewMatrix;
	static Matrix ProjectionMatrix;
	static Vec3 CameraPos;


	Matrix mView;
	Matrix mProj;
	Vec3 mPosition;


	float mSpeed = 1.0f;
	POINT mPos = {0,0};



	uint32 mCullingMask=0;
	vector<shared_ptr<DefaultObject>> mObjectVector;
	vector<shared_ptr<DefaultObject>> mBillboardObjectVector;
	vector<shared_ptr<DefaultObject>> mParticleSystems;
	//Frustum 
	BoundingFrustum mFrustum;
};

