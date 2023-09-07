#include "pch.h"
#include "MathUtils.h"

bool MathUtils::IsPointInSphere(const Point3D& point, const Sphere3D sphere)
{
	Point3D center = sphere.position;

	float r = sphere.radius;


	float dist = (point - center).LengthSquared();
	return dist <= r;


}

Point3D MathUtils::ClosestPoint(const Sphere3D& sphere, const Point3D& point)
{
	Point3D center = sphere.position;

	float r = sphere.radius;

	Vec3 dir = point - center;
	dir.Normalize();

	return center + r * dir;

}






bool MathUtils::IsPointInAABB(const Point3D& point, const AABB3D& aabb)
{
	Vec3 min = AABB3D::GetMin(aabb);
	Vec3 max = AABB3D::GetMax(aabb);


	if (point.x < min.x || point.y < min.y || point.z < min.z)
		return false;
	if (point.x > max.x || point.y > max.y || point.z > max.z)
		return false;

	return true;

}





Point3D MathUtils::ClosestPoint(const AABB3D& aabb, const Point3D& point)
{
	Point3D result = point;
	Point3D minPt = AABB3D::GetMin(aabb);
	Point3D maxPt = AABB3D::GetMax(aabb);

	result.x = max(result.x, minPt.x);
	result.y = max(result.y, minPt.y);
	result.z = max(result.z, minPt.z);

	result.x = min(result.x, maxPt.x);
	result.y = min(result.y, maxPt.y);
	result.z = min(result.z, maxPt.z);

	return result;
}





bool MathUtils::IsPointInOBB(const Point3D& point, const OBB3D& obb)
{
	Point3D center = obb.position;

	Vec3 dir = point - center;

	vector<Vec3> axisOfOBB;
	axisOfOBB.push_back(obb.orientation.Right());
	axisOfOBB.push_back(obb.orientation.Up());
	axisOfOBB.push_back(obb.orientation.Backward());


	vector<float> size;
	size.push_back(obb.size.x);
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);

	for (int i = 0; i < 3; i++) {
		float dist = dir.Dot(axisOfOBB[i]);

		if (dist > size[i]) {
			return false;
		}

		if (dist < -size[i]) {
			return false;
		}
			
	
	}
	return true;
}


Point3D MathUtils::ClosestPoint(const OBB3D& obb, const Point3D& point)
{

	Point3D center = obb.position;
	Vec3 dir = point - obb.position;

	vector<Vec3> axis;
	axis.push_back(obb.orientation.Right());
	axis.push_back(obb.orientation.Up());
	axis.push_back(obb.orientation.Backward());

	vector<float> size;
	size.push_back(obb.size.x);
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);

	for (int i = 0; i < 3; ++i)
	{
		float distance = dir.Dot(axis[i]);

		//거리르 clamp
		if (distance > size[i])
			distance = size[i];

		if (distance < -size[i])
			distance = -size[i];

		center = center + (axis[i] * distance);
	}

	return center;
}

bool MathUtils::IsPointOnPlane(const Point3D& point, const Plane3D& plane)
{
	float dist = point.Dot(plane.normal);

	return dist == plane.distance;

}


Point3D MathUtils::ClosestPoint(const Plane3D& plane, const Point3D& point)
{
	float dist = point.Dot(plane.normal);
	float distance = dist - plane.distance;

	return point - distance * plane.normal;

}




bool MathUtils::IsPointOnLine(const Point3D& point, const Line3D& line)
{
	Point3D closest = ClosestPoint(line, point);

	float dist = (point - closest).LengthSquared();
	return dist == 0.f;

}


bool MathUtils::IsPointOnRay(const Point3D& point, const Ray3D& ray)
{
	if (point == ray.origin)
		return true;
	//ray.direction 은 단위 백터
	Vec3 norm = point - ray.origin;
	norm.Normalize();

	float diff = norm.Dot(ray.direction);
	return diff == 1.0f;
}



Point3D MathUtils::ClosestPoint(const Line3D& line, const Point3D& point)
{
	Vec3 line_vec = line.end - line.start;

	line_vec.Normalize();
	float t = (point - line.start).Dot(line_vec);

	t = fmaxf(t, 0.0f); // Clamp to 0
	t = fminf(t, 1.0f); // Clamp to 1

	return line.start + line_vec * t;
}

Point3D MathUtils::ClosestPoint(const Ray3D& ray, const Point3D& point)
{

	float t = (point - ray.origin).Dot(ray.direction);
	//clamping
	t = fmaxf(t, 0.0f);

	return Point3D(ray.origin + ray.direction * t);
}

