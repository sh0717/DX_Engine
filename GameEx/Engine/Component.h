#pragma once

class DefaultObject;
class Transform;

enum class eComponentType : uint8 {

	Transform,
	MeshRenderer,
	ModelRenderer,
	ModelAnimator,
	BillboardRenderer,
	Camera ,
	Light,
	Collider,
	Animator,
	Terrain,

	Button,
	Wave,
	ParticleSystem,




	Script,
	END,//4

};
enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(eComponentType::END)-1 //3
};


class Component
{
public:
	Component(eComponentType type);
	virtual ~Component();

	virtual void Init() {};
	virtual void Start() {};
	
	virtual void Update() {};

	virtual void LateUpdate() {};
	virtual void FixedUpdate() {};

	eComponentType GetType() { return  mType; }

	inline shared_ptr<DefaultObject> GetGameObject() { return mOwner.lock(); }
	shared_ptr<Transform> GetTransform();
private:
	friend class DefaultObject;
	void SetGameObject(shared_ptr<DefaultObject> gameObject) { mOwner = gameObject; }


protected:
	eComponentType mType;
	weak_ptr<DefaultObject> mOwner;
};