#include "pch.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider()
	:BaseCollider(eColliderType::Sphere)
{

}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::Update()
{
	mBoundingSphere.Center = GetTransform()->GetPosition();

	Vec3  scale = GetTransform()->GetScale();

	mBoundingSphere.Radius = mRadius * (max(max(scale.x, scale.y), scale.z));
}

bool SphereCollider::IsIntersects(Ray& ray, OUT float& distance)
{
	return mBoundingSphere.Intersects(ray.position,ray.direction, distance);
}

bool SphereCollider::IsIntersects(shared_ptr<BaseCollider>& other)
{






	return false;
}
