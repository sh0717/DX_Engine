#include "pch.h"
#include "CameraScript.h"
#include "Terrain.h"
void CameraScript::Start()
{



}

void CameraScript::Update()
{
	if (INPUT->GetButton(KEY_TYPE::Z)) {
		mode = 0;
	}
	else if (INPUT->GetButton(KEY_TYPE::C)) {
		mode = 1;
	}

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


	if (mode == 0) {

		auto& terrain_vector = SCENE->GetCurrentScene()->GetTerrain();

		if (terrain_vector.size() > 0) {
			auto terrain = terrain_vector[0]->GetTerrain();
			Vec3 pos = GetTransform()->GetPosition();
			pos.y = terrain->GetHeight(pos.x, pos.z) + 1.8f;
			GetTransform()->ChangeHeight(pos.y);
		}
	}
}
