#include "pch.h"
#include "AABBCollider.h"

AABBCollider::AABBCollider()
	:BaseCollider(eColliderType::AABB)
{

}

AABBCollider::~AABBCollider()
{

}

void AABBCollider::Update()
{
	Vec3 pos = GetTransform()->GetPosition();
	Vec3 scale = GetTransform()->GetScale();

	mBoundingBox.Center = pos;
	mBoundingBox.Extents = scale;
}

bool AABBCollider::IsIntersects(Ray& ray, OUT float& distance)
{
	return mBoundingBox.Intersects(ray.position, ray.direction, distance);
}

bool AABBCollider::IsIntersects(shared_ptr<BaseCollider>& other)
{



	return false;
}
