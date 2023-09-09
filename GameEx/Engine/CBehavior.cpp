#include "pch.h"
#include "CBehavior.h"
#include "Terrain.h"
CBehavior::CBehavior()
{

}

CBehavior::~CBehavior()
{

}

void CBehavior::Init()
{

}

void CBehavior::Update()
{
	auto cam = SCENE->GetCurrentScene()->GetMainCamera();
	GetTransform()->SetPosition(cam->GetTransform()->GetPosition() + cam->GetTransform()->GetLook()*5.f-Vec3(0.f,0.5f,0.f));

	Vec3 CurPos = GetTransform()->GetPosition();
	auto terr = SCENE->GetCurrentScene()->GetTerrain();
	if (terr.size() > 0) {
		CurPos.y = terr[0]->GetTerrain()->GetHeight(CurPos.x,CurPos.z)-0.2f;
	}

	GetTransform()->SetPosition(CurPos);
	//GetTransform()->SetPosition(Vec3(10.f, -10.f, 10.f));


	//내가 보는방향 
	Vec3 look = cam->GetTransform()->GetLook();

	//캐릭터가 보는방향 
	Vec3 ch_look = Vec3(0.f, 0.f, 1.f);


	look.y = 0.f;
	look.Normalize();




	Vec3 up = Vec3(0.f, 1.f, 0.f);
	Vec3 myPos = GetTransform()->GetRotation();
	Matrix lookMatrix = Matrix::CreateWorld(myPos, look, up);

	Vec3 S, T;
	Quaternion R;
	lookMatrix.Decompose(S, R, T);

	Vec3 rot = Transform::ToEulerAngles(R);

	GetTransform()->SetRotation(rot);



	if (INPUT->GetButton(KEY_TYPE::W)) {
		GetTransform()->mode = eTransformMode::Moving;
	}
	else if (INPUT->GetButton(KEY_TYPE::S)) {
		GetTransform()->mode = eTransformMode::Moving;
	}
	else if (INPUT->GetButton(KEY_TYPE::A)) {
		GetTransform()->mode = eTransformMode::Moving;
	}
	else if (INPUT->GetButton(KEY_TYPE::D)) {
		GetTransform()->mode = eTransformMode::Moving;
	}
	else {
		GetTransform()->mode = eTransformMode::Idle;
	}


}
