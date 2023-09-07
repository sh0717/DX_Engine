#include "pch.h"
#include "Model.h"
#include <filesystem>
void ModelMesh::CreateBuffers()
{

	vertexBuffer = make_shared<VertexBuffer>();
	vertexBuffer->CreateBuffer(geometry->GetVertices());

	indexBuffer = make_shared<IndexBuffer>();
	indexBuffer->CreateBuffer(geometry->GetIndices());


}




std::shared_ptr<ModelKeyframe> ModelAnimation::GetKeyframe(const wstring& name)
{
	auto findIt = keyframes.find(name);
	if (findIt == keyframes.end())
		return nullptr;

	return findIt->second;
}


Model::Model()
{
	
}

Model::~Model()
{

}

void Model::ReadMaterial(wstring filename)
{
	wstring fullPath = mTextureBasePath + filename + L".xml";
	auto parentPath = filesystem::path(fullPath).parent_path();

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = document->LoadFile(Utils::ToString(fullPath).c_str());
	assert(error == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement* root = document->FirstChildElement();
	tinyxml2::XMLElement* materialNode = root->FirstChildElement();

	while (materialNode) {
		shared_ptr<Material> material = make_shared<Material>();
		tinyxml2::XMLElement* node = nullptr;

		node = materialNode->FirstChildElement();
		material->SetName(Utils::ToWString(node->GetText()));
		//material 의 name 


		node = node->NextSiblingElement();
		
		//diffuse texture file name 이 존재한다면?
		if (node->GetText()) {
			
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0) 
			{
				auto texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetDiffuseTexture(texture);
			}
		}


		//specular Texture file name 이 존재한다면?
		//material 에 추가해주자 
		node = node->NextSiblingElement();
		if (node->GetText()) {

			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetSpecularTexture(texture);
			}
		}

		//normal Texture file name 이 존재한다면?
		//material 에 추가해주자 
		node = node->NextSiblingElement();
		if (node->GetText()) {

			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = RESOURCES->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetNormalTexture(texture);
			}
		}


		Color color;

		//Ambient 결정
		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetMaterialDesc().Ambient = color;


		//Diffuse 결정 
		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetMaterialDesc().Diffuse = color;


		//specular 결정

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetMaterialDesc().Specular = color;

		//emissive 결정
		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetMaterialDesc().Emissive = color;


		mMaterials.push_back(material);
		materialNode = materialNode->NextSiblingElement();
	}


	BindCacheInfo();
}

void Model::ReadModel(wstring filename)
{
	wstring fullPath = mModelBasePath + filename + L".mesh";
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	//write 한 순서대로 읽어주면 된다.
	//1. bones;

	{
		uint32 count = file->Read<uint32>();

		for (uint32 i = 0; i < count; ++i) {
			shared_ptr<ModelBone> bone = make_shared<ModelBone>();

			bone->index = file->Read<int32>();
			bone->name = Utils::ToWString(file->Read<string>());
			bone->parentIndex = file->Read<int32>();
			bone->transform = file->Read<Matrix>();

			mBones.push_back(bone);
		}
		
	}


	//2.mesh
	{
		uint32 count = file->Read<uint32>();

		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ModelMesh> mesh = make_shared<ModelMesh>();

			mesh->name = Utils::ToWString(file->Read<string>());
			mesh->boneIndex = file->Read<int32>();

			// Material
			mesh->materialName = Utils::ToWString(file->Read<string>());

			//VertexData
			{
				const uint32 count = file->Read<uint32>();
				vector<ModelVertexType> vertices;
				vertices.resize(count);

				void* data = vertices.data();
				file->Read(&data, sizeof(ModelVertexType) * count);
				mesh->geometry->AddVertices(vertices);
			}

			//IndexData
			{
				const uint32 count = file->Read<uint32>();

				vector<uint32> indices;
				indices.resize(count);

				void* data = indices.data();
				file->Read(&data, sizeof(uint32) * count);
				mesh->geometry->AddIndices(indices);
			}

			mesh->CreateBuffers();

			mMeshes.push_back(mesh);
		}
	
	}
	BindCacheInfo();


}

void Model::ReadAnimation(wstring filename)
{
	//애니메이션은 .clip 파일로 저장
	wstring fullPath = mModelBasePath + filename + L".clip";

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	shared_ptr<ModelAnimation> animation = make_shared<ModelAnimation>();

	//1.name 2.duration 3.frameRate 4.frameCount

	animation->name = Utils::ToWString(file->Read<string>());
	animation->duration = file->Read<float>();
	animation->frameRate = file->Read<float>();
	animation->frameCount = file->Read<uint32>();


	// 키 프레임이 몇개고
	uint32 keyframesCnt = file->Read<uint32>();

	for (uint32 i = 0; i < keyframesCnt; ++i) {
	
		shared_ptr<ModelKeyframe> keyframe = make_shared<ModelKeyframe>();

		//1.bone 2. size 
		keyframe->boneName = Utils::ToWString(file->Read<string>());

		uint32 size = file->Read<uint32>();

		if (size > 0) {
			keyframe->transforms.resize(size);
			void* ptr = &keyframe->transforms[0];
			file->Read(&ptr, sizeof(ModelKeyframeData) * size);
		
		}

		animation->keyframes[keyframe->boneName] = keyframe;
	}

	mAnimations.push_back(animation);

}

std::shared_ptr<Material> Model::GetMaterialByName(const wstring& name)
{

	for (shared_ptr<Material> m : mMaterials) {
	
		if (m->GetName() == name) {
			return m;
		}
	}
	return nullptr;

}

std::shared_ptr<ModelMesh> Model::GetMeshByName(const wstring& name)
{
	for (auto mesh : mMeshes) {
		if (mesh->name == name) {
			return mesh;
		}
	}
	return nullptr;
}

std::shared_ptr<ModelBone> Model::GetBoneByName(const wstring& name)
{
	for (auto bone : mBones) {
		if (bone->name == name) {
			return bone;
		}
	}
	return nullptr;
}

std::shared_ptr<ModelAnimation> Model::GetAnimationByName(wstring name)
{

	for (auto anim: mAnimations) {
	
		if (anim->name == name) {
			return anim;
		}
	}

	return nullptr;
}

void Model::BindCacheInfo()
{

	for (const auto& mesh : mMeshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->material != nullptr)
			continue;

		mesh->material = GetMaterialByName(mesh->materialName);
	}

	// Mesh에 Bone 캐싱
	for (const auto& mesh : mMeshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->bone != nullptr)
			continue;

		mesh->bone = GetBoneByIndex(mesh->boneIndex);
	}

	// Bone 계층 정보 채우기
	if (mRoot == nullptr && mBones.size() > 0)
	{
		mRoot = mBones[0];

		for (const auto& bone : mBones)
		{
			if (bone->parentIndex >= 0)
			{
				bone->parent = mBones[bone->parentIndex];
				bone->parent->children.push_back(bone);
			}
			else
			{
				bone->parent = nullptr;
			}
		}
	}


}







//////////////////////////////

