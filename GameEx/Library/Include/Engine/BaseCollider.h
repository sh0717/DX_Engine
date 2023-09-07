#pragma once
/// <summary>
/// 사실 이 콜라이더 클래스는 
/// DriectXCollision의 wrapper 이다 
/// </summary>
enum class eColliderType
{
	Sphere,
	AABB,
	OBB,
};



class BaseCollider:public Component
{
public:

	BaseCollider(eColliderType colliderType);
	virtual ~BaseCollider();
	virtual bool IsIntersects(Ray& ray, OUT float& distance) = 0;
	virtual bool IsIntersects(shared_ptr<BaseCollider>& other) = 0;

	eColliderType GetColliderType() { return mColliderType; }

protected:
	eColliderType mColliderType;





};

