#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void Init() override;
	virtual void Update() override;

	void UpdateTransform();

	///for Camera
	virtual void Walk(float d) {}
	virtual void Strafe(float d) {}
	virtual void Pitch(float angle) {}
	virtual void RotateY(float angle) {}


	// Local
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }
	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation; UpdateTransform(); }
	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) { _localPosition = localPosition; UpdateTransform(); }

	// World
	Vec3 GetScale() { return _scale; }
	void SetScale(const Vec3& scale);
	Vec3 GetRotation() { return _rotation; }
	void SetRotation(const Vec3& rotation);

	Quaternion GetRotationQuat() { return XMQuaternionRotationRollPitchYawFromVector(Vec4(_rotation.x,_rotation.y,_rotation.z, 0.f)); }
	Vec3 GetPosition() { return _position; }
	void SetPosition(const Vec3& position);

	virtual Vec3 GetRight() { return _matWorld.Right(); }
	virtual Vec3 GetUp() { return _matWorld.Up(); }
	virtual Vec3 GetLook() { return _matWorld.Backward(); }

	Matrix GetWorldMatrix() { return _matWorld; }
	//TODO world로부터 나머지 데이터 정리 
	void SetWorldMatrix(Matrix _world) { _matWorld = _world;}

	// 계층 관계
	bool HasParent() { return _parent != nullptr; }

	shared_ptr<Transform> GetParent() { return _parent; }
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }

	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
	void AddChild(shared_ptr<Transform> child) { _children.push_back(child); }




protected:
	Vec3 _localScale = { 1.f, 1.f, 1.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localPosition = { 0.f, 0.f, 0.f };

	// Cache
	Matrix _matLocal = Matrix::Identity;
	Matrix _matWorld = Matrix::Identity;

	Vec3 _scale;
	Vec3 _rotation;
	Vec3 _position;

	Vec3 _right;
	Vec3 _up;
	Vec3 _look;

private:
	shared_ptr<Transform> _parent;
	vector<shared_ptr<Transform>> _children;
};



class CameraTransform:public Transform 
{
public:
	CameraTransform();
	~CameraTransform();

	virtual void Update() override;
	virtual void Walk(float d);
	virtual void Strafe(float d);
	virtual void Pitch(float angle);
	virtual void RotateY(float angle);

	virtual Vec3 GetPosition() { return _position; }
	virtual Vec3 GetRight() { return _right; }
	virtual Vec3 GetUp() { return _up; }
	virtual Vec3 GetLook() { return _look; }

};