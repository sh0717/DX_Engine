
#include "pch.h"
#include "CubeBehavior.h"

void CubeBehavior::Start()
{

}

void CubeBehavior::Update()
{

}

void CubeBehavior::LateUpdate()
{
	GetTransform()->SetPosition(SCENE->GetCurrentScene()->GetMainCamera()->GetCamera()->GetMyPos());

}


