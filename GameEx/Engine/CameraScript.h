#pragma once
class CameraScript:public MonoBehavior
{
public:

	virtual void Start() override;

	virtual void Update() override;

	virtual void LateUpdate() override;
	float mSpeed = 10.f;

	POINT mPos = {0,0};


};

