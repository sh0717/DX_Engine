#include "pch.h"
#include "Component.h"
#include "DefaultObject.h"

Component::Component(eComponentType type):mType(type)
{

}

Component::~Component()
{

}

std::shared_ptr<Transform> Component::GetTransform()
{
	return mOwner.lock()->GetTransform();
}
