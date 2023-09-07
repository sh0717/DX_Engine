#include "pch.h"
#include "OBBCollider.h"









OBBCollider::OBBCollider()
	:BaseCollider(eColliderType::OBB)
{

}

OBBCollider::~OBBCollider()
{

}

void OBBCollider::Update()
{
	Vec3 pos = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();

	mBoundingBox.Center = pos;
	mBoundingBox.Extents = scale;
	mBoundingBox.Orientation = GetTransform()->GetRotationQuat();
}

bool OBBCollider::IsIntersects(Ray& ray, OUT float& distance)
{
	return mBoundingBox.Intersects(ray.position, ray.direction, distance);
}

bool OBBCollider::IsIntersects(shared_ptr<BaseCollider>& other)
{





	return false;
}
