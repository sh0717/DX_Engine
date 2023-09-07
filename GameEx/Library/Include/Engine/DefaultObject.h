#pragma once
#include "Component.h"

class MeshRenderer;
class ModelRenderer;
class ModelAnimator;
class BillboardRenderer;

class Camera;
class MonoBehavior;
class Light;
class BaseCollider;
class Wave;
class Button;
class Terrain;
class ParticleSystem;

class DefaultObject:public enable_shared_from_this<DefaultObject>
{
public:


	DefaultObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~DefaultObject();

	void Init();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();


	//void Render(shared_ptr<Pipeline> pipeline);

	shared_ptr<Component> GetFixedComponent(eComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Transform> GetOrAddTransform();
	


	shared_ptr<Camera> GetCamera();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<ModelRenderer> GetModelRenderer();
	shared_ptr<ModelAnimator> GetModelAnimator();
	shared_ptr<BillboardRenderer> GetBillboardRenderer();

	shared_ptr<Light> GetLight();
	shared_ptr<Button> GetButton();
	shared_ptr<BaseCollider> GetCollider();
	shared_ptr<Wave> GetWave();
	shared_ptr<Terrain> GetTerrain();
	shared_ptr<ParticleSystem> GetParticleSystem();


	void AddComponent(shared_ptr<Component> component);


	inline void SetLayerIndex(uint8 idx) { mLayerIndex = idx; }
	inline uint8 GetLayerIndex() { return mLayerIndex; }
private:
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11DeviceContext> mDeviceContext;


	uint8 mLayerIndex=0;

protected:
	
	std::array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> mComponents;
	std::vector<shared_ptr<MonoBehavior>> mBehaviors;
};

