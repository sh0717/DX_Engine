#pragma once
#include "ResourceBase.h"

class Mesh;
class Material;
class Shader;
class Animation;
class Texture;


class ResourceManager

{
	DECLARE_SINGLE(ResourceManager);

public:

	ResourceManager(ComPtr<ID3D11Device> device);
	~ResourceManager();

	void Init();

	//update 할건 따로 없다 .

	template<typename T>
	RESOURCE_TYPE GetResourceType();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	shared_ptr<Texture> GetOrAddTexture(const wstring& key, const wstring& path);


private:
	//처음에 init 할 때 넣어놓을 리소스들 만드는 함수 





	void CreateDefaultTexture();
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateDefaultAnimation();


private:

	ComPtr<ID3D11Device> mDevice;

	using KeyObjMap = map<wstring, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> mResources;

};

template<typename T>
RESOURCE_TYPE ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Mesh>)
		return RESOURCE_TYPE::MESH;
	if (std::is_same_v<T, Shader>)
		return RESOURCE_TYPE::SHADER;
	if (std::is_same_v<T, Texture>)
		return RESOURCE_TYPE::TEXTURE;
	if (std::is_same_v<T, Material>)
		return RESOURCE_TYPE::MATERIAL;
	if (std::is_same_v<T, Animation>)
		return RESOURCE_TYPE::ANIMATION;

	assert(false);
	return RESOURCE_TYPE::NONE;
}

template<typename T>
shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring& path)
{
	auto objectType = GetResourceType<T>();
	KeyObjMap& keyObjMap = mResources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

template<typename T>
bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	auto resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = mResources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;
	return true;
}

template<typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	auto resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = mResources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

