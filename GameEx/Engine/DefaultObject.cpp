#include "pch.h"
#include "DefaultObject.h"
//#include "MeshRenderer.h"
#include "BillboardRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "ModelRenderer.h"
#include "BaseCollider.h"
#include "Wave.h"
#include "Terrain.h"
#include "Button.h"
#include "ParticleSystem.h"
DefaultObject::DefaultObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	:mDevice(device),mDeviceContext(deviceContext)
{
	
	
}


DefaultObject::~DefaultObject()
{

}

void DefaultObject::Init()
{
	for (shared_ptr<Component>& component : mComponents)
	{
		if (component)
			component->Init();
	}

	for (shared_ptr<MonoBehavior>& script : mBehaviors)
	{
		script->Init();
	}
}

void DefaultObject::Start()
{
	for (shared_ptr<Component>& component : mComponents)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehavior>& script : mBehaviors)
	{
		script->Start();
	}
}

void DefaultObject::Update()
{
	/*Vec3 pos = mTransformComponent->GetPosition();
	pos.x += 0.01;

	mTransformComponent->SetPosition(pos);

	mTransform.matWorld = mTransformComponent->GetWorldMatrix();
	mCB_transform->Update(mTransform);*/


	for (shared_ptr<Component>& component : mComponents)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehavior>& script : mBehaviors)
	{
		script->Update();
	}


}

void DefaultObject::LateUpdate()
{
	for (shared_ptr<Component>& component : mComponents)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehavior>& script : mBehaviors)
	{
		script->LateUpdate();
	}
}

void DefaultObject::FixedUpdate()
{
	for (shared_ptr<Component>& component : mComponents)
	{
		if (component)
			component->FixedUpdate();
	}

	for (shared_ptr<MonoBehavior>& script : mBehaviors)
	{
		script->FixedUpdate();
	}
}

//void DefaultObject::Render(shared_ptr<Pipeline> pipeline)
//{
	
//}

std::shared_ptr<Component> DefaultObject::GetFixedComponent(eComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return mComponents[index];
}

std::shared_ptr<Transform> DefaultObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::Transform);
	return static_pointer_cast<Transform>(component);
}

std::shared_ptr<Transform> DefaultObject::GetOrAddTransform()
{
	if (GetTransform() == nullptr)
	{
		shared_ptr<Transform> transform = make_shared<Transform>();
		AddComponent(transform);
	}

	return GetTransform();
}
//
std::shared_ptr<Camera> DefaultObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::Camera);
	return static_pointer_cast<Camera>(component);
}

std::shared_ptr<MeshRenderer> DefaultObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::MeshRenderer);
	return static_pointer_cast<MeshRenderer>(component);
}

std::shared_ptr<ModelRenderer> DefaultObject::GetModelRenderer()
{

	shared_ptr<Component> component = GetFixedComponent(eComponentType::ModelRenderer);
	return static_pointer_cast<ModelRenderer>(component);

}

std::shared_ptr<ModelAnimator> DefaultObject::GetModelAnimator()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::ModelAnimator);
	return static_pointer_cast<ModelAnimator>(component);
}

std::shared_ptr<BillboardRenderer> DefaultObject::GetBillboardRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::BillboardRenderer);
	return static_pointer_cast<BillboardRenderer>(component);
}

std::shared_ptr<Light> DefaultObject::GetLight()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::Light);
	return static_pointer_cast<Light>(component);
}

std::shared_ptr<Button> DefaultObject::GetButton()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::Button);
	return static_pointer_cast<Button>(component);
}

std::shared_ptr<BaseCollider> DefaultObject::GetCollider()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::Collider);
	return static_pointer_cast<BaseCollider>(component);
}

std::shared_ptr<Wave> DefaultObject::GetWave()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::Wave);
	return static_pointer_cast<Wave>(component);
}

std::shared_ptr<Terrain> DefaultObject::GetTerrain()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::Terrain);
	return static_pointer_cast<Terrain>(component);
}

std::shared_ptr<ParticleSystem> DefaultObject::GetParticleSystem()
{
	shared_ptr<Component> component = GetFixedComponent(eComponentType::ParticleSystem);
	return static_pointer_cast<ParticleSystem>(component);
}

void DefaultObject::AddComponent(shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());
	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		mComponents[index] = component;
	}
	else
	{
		mBehaviors.push_back(static_pointer_cast<MonoBehavior>(component));
	}
}
