#include "pch.h"
#include "BaseCollider.h"

BaseCollider::BaseCollider(eColliderType colliderType)
	:Component(eComponentType::Collider),mColliderType(colliderType)
{

}

BaseCollider::~BaseCollider()
{

}
