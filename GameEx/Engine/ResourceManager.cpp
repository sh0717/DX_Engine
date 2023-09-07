#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include <filesystem>
//#include "Mesh.h"

//#include "Material.h"
ResourceManager::ResourceManager(ComPtr<ID3D11Device> device)
	:mDevice(device)
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Init()
{
	CreateDefaultTexture();
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateDefaultAnimation();
}

std::shared_ptr<Texture> ResourceManager::GetOrAddTexture(const wstring& key, const wstring& path)
{
	shared_ptr<Texture> texture = Get<Texture>(key);

	if (filesystem::exists(filesystem::path(path)) == false)
		return nullptr;

	texture = Load<Texture>(key, path);

	if (texture == nullptr)
	{
		texture = make_shared<Texture>();
		texture->Load(path);
		Add(key, texture);
	}

	return texture;
}

void ResourceManager::CreateDefaultTexture()
{
	

}

void ResourceManager::CreateDefaultMesh()
{
	
	shared_ptr<Mesh> CubeMesh = make_shared<Mesh>();
	CubeMesh->CreateCube();
	Add(L"Cube", CubeMesh);

	shared_ptr<Mesh> SphereMesh = make_shared<Mesh>();
	SphereMesh->CreateSphere();
	Add(L"Sphere", SphereMesh);

	shared_ptr<Mesh> quadMesh = make_shared<Mesh>();
	quadMesh->CreateQuad();
	Add(L"Quad", quadMesh);

}

void ResourceManager::CreateDefaultShader()
{
	/*{
		auto mVS = make_shared<VertexShader>(mDevice);
		mVS->Create(L"Vertex.hlsl", "VS", "vs_5_0");

		auto mInputLayout = make_shared<InputLayout>(mDevice);
		mInputLayout->Create(VertexTextureData::descs, mVS->GetBlob());

		auto mPS = make_shared<PixelShader>(mDevice);
		mPS->Create(L"Vertex.hlsl", "PS", "ps_5_0");

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->SetName(L"Vertex");
		shader->SetInputLayout(mInputLayout);
		shader->SetVertexShader(mVS);
		shader->SetPixelShader(mPS);
		Add(shader->GetName(), shader);


	}*/





}

void ResourceManager::CreateDefaultMaterial()
{
	/*shared_ptr<Material> material = make_shared<Material>();
	material->SetName(L"Default");
	material->SetShader(Get<Shader>(L"Vertex"));
	material->SetTexture(Get<Texture>(L"Skeleton"));
	Add(material->GetName(), material);*/
}

void ResourceManager::CreateDefaultAnimation()
{

}
