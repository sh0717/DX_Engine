#include "pch.h"
#include "CameraScript.h"

void CameraScript::Start()
{



}

void CameraScript::Update()
{


}

void CameraScript::LateUpdate()
{

	float dt = TIME->GetDeltaTime();

	Vec3 CurPos = GetTransform()->GetPosition();

	if (INPUT->GetButton(KEY_TYPE::W)) {
		GetTransform()->Walk(mSpeed * dt);
	}
	if (INPUT->GetButton(KEY_TYPE::S)) {
		GetTransform()->Walk(-mSpeed * dt);
	}
	if (INPUT->GetButton(KEY_TYPE::A)) {
		GetTransform()->Strafe(-mSpeed * dt);
	}
	if (INPUT->GetButton(KEY_TYPE::D)) {
		GetTransform()->Strafe(mSpeed * dt);
	}

	


	//if (INPUT->GetButton(KEY_TYPE::Q))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.x += dt * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::E))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.x -= dt * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::Z))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.y += dt * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::C))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.y -= dt * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	LONG x = INPUT->GetMousePos().x;
	LONG y = INPUT->GetMousePos().y;

	if (INPUT->GetButton(KEY_TYPE::LBUTTON)) {




		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mPos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mPos.y));

		GetTransform()->Pitch(dy);
		GetTransform()->RotateY(dx);


	}
	mPos.x = x;
	mPos.y = y;

}
