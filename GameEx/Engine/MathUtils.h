#pragma once


// *************
// Point3D
// *************
using Point3D = Vec3;

// *************
// Line3D
// *************

struct Line3D
{
	Point3D start = Point3D(0.f);
	Point3D end = Point3D(0.f);

	float Length() { return Vec3::Distance(start, end); }
	float LengthSq() { return Vec3::DistanceSquared(start, end); }
};

// *************
// Ray3D
// *************

struct Ray3D
{
	Point3D origin = Point3D(0.f);
	Vec3 direction = Vec3(0.f);

	void NormalizeDirection() { direction.Normalize(); }

	static Ray3D FromPoints(const Point3D& from, const Point3D& to) { return Ray3D{ from, to - from }; }
};

// *************
// Sphere3D
// *************

struct Sphere3D
{
	Point3D position;
	float radius;
};

// *************
// AABB
// *************

struct AABB3D
{
	Point3D position = Vec3(0.f);
	Vec3 size = Vec3(1.f, 1.f, 1.f);

	static Vec3 GetMin(const AABB3D& aabb)
	{
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;
		return Vec3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
	}

	static Vec3 GetMax(const AABB3D& aabb)
	{
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;
		return Vec3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
	}

	static AABB3D FromMinMax(const Vec3& min, const Vec3& max)
	{
		return AABB3D((min + max) / 2, (max - min) / 2);
	}
};

// *****************
// OBB
// *****************

struct OBB3D
{
	Point3D position;
	Vec3 size;
	Matrix orientation;
	// Vec4 quaternion;
	// Vec3 rotation;
};

// *****************
// Plane3D
// *****************

// 삼각형 (정점3개)
// 노멀 + 정점 1개
// 노멀 + 거리
struct Plane3D
{
	Vec3 normal;
	float distance;
};

// *****************
// Triangle3D
// *****************

struct Triangle3D
{
	union
	{
		struct
		{
			Point3D a;
			Point3D b;
			Point3D c;
		};
		Point3D points[3];
		float values[9];
	};
};

struct Interval3D
{
	float min;
	float max;
};



class MathUtils
{
public:
	/// <summary>
	/// Point test 
	/// </summary>
	/// 
	
	//point 가 sphere 이내에 있는지 체크해준다. 
	static bool IsPointInSphere(const Point3D& point, const Sphere3D sphere);

	//point 에서 sphere 에 가장 가까운 점을 returbn  구 표면 위의 점이 나온다.
	static Point3D ClosestPoint(const Sphere3D& sphere, const Point3D& point);


	//aabb 큐브에 point 가 있는지 체크해준다.
	static bool IsPointInAABB(const Point3D& point, const AABB3D& aabb);


	///aabb 큐브에서 가장 가까운 점을 리턴 
	static Point3D ClosestPoint(const AABB3D& aabb, const Point3D& point);


	// OBB2Point
	static bool IsPointInOBB(const Point3D& point, const OBB3D& obb);
	static Point3D ClosestPoint(const OBB3D& obb, const Point3D& point);


	static bool IsPointOnPlane(const Point3D& point, const Plane3D& plane);
	static Point3D ClosestPoint(const Plane3D& plane, const Point3D& point);



	static bool IsPointOnLine(const Point3D& point, const Line3D& line);
	static Point3D ClosestPoint(const Line3D& line, const Point3D& point);

	
	static bool IsPointOnRay(const Point3D& point, const Ray3D& ray);
	static Point3D ClosestPoint(const Ray3D& ray, const Point3D& point);



	////Intersection
	static bool Intersection_SphereSphere(const Sphere3D& s1, const Sphere3D& s2);
	static bool Intersection_SphereAABB(const Sphere3D& sphere, const AABB3D& aabb);
	static bool Intersection_SphereOBB(const Sphere3D& sphere, const OBB3D& obb);
	static bool Intersection_SpherePlane(const Sphere3D& sphere, const Plane3D& plane);
	static bool Intersection_AABBAABB(const AABB3D& aabb1, const AABB3D& aabb2);

	static Interval3D GetInterval(const AABB3D& aabb, const Vec3& axis);
	static Interval3D GetInterval(const OBB3D& obb, const Vec3& axis);
	static bool OverlapOnAxis(const AABB3D& aabb, const OBB3D& obb, const Vec3& axis);
	static bool Intersection_AABBOBB(const AABB3D& aabb, const OBB3D& obb);

	static bool OverlapOnAxis(const OBB3D& obb1, const OBB3D& obb2, const Vec3& axis);
	static bool Intersection_AABBPlane(const AABB3D& aabb, const Plane3D& plane);
	static bool Intersection_OBBOBB(const OBB3D& obb1, const OBB3D& obb2);
	static bool Intersection_PlanePlane(const Plane3D& plane1, const Plane3D& plane2);

};

