#include "pch.h"
#include "Converter.h"
#include <filesystem>
Converter::Converter()
{
	mImporter = make_shared<Assimp::Importer>();
}

Converter::~Converter()
{

}

void Converter::ReadAssetFile(wstring file)
{
	wstring fileStr = mAssetBasePath + file;
	//asset �� �����ϴ� ���� ���


	auto p = std::filesystem::path(fileStr);
	assert(filesystem::exists(p));

	mScene = mImporter->ReadFile(
		Utils::ToString(fileStr),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	assert(mScene != nullptr);


}

void Converter::ExportModelData(wstring savePath)
{
	//�𵨵����͸� ������ ��� == finalPath
	wstring finalPath = mModelBasePath + savePath + L".mesh";
	

	//���⼭ ��������� �о vector�� ä���. 
	ReadModelData(mScene->mRootNode, -1, -1);
	
	//mmeshes , mBones ä�� ����

	//�� �޽����� ���� �����͸� ä���ش�.
	ReadSkinData();

	
	WriteModelFile(finalPath);

}

void Converter::ExportMaterialData(wstring savePath)
{
	wstring finalPath = mTextureBasePath + savePath + L".xml";

	//scene ���� material �о mMaterials �� ä�����
	ReadMaterialData();

	//finalPath ��  xml ��������
	WriteMaterialData(finalPath);


}

void Converter::ExportAnimationData(wstring savePath, uint32 index /*= 0*/)
{
	//���ϸ��̼� �����ʹ� Ȯ���� .clip
	wstring finalPath = mModelBasePath + savePath + L".clip";
	assert(index < mScene->mNumAnimations);

	shared_ptr<asAnimation> animation = ReadAnimationData(mScene->mAnimations[index]);
	WriteAnimationData(animation, finalPath);


}

void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)
{
	//bone ���� idx �� ����  parent �� ���� 
	
	shared_ptr<asBone> bone = make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();


	Matrix transform(node->mTransformation[0]);
	//�̰Ŵ� relative transformation
	bone->transform = transform.Transpose();


	Matrix matParent = Matrix::Identity;
	//�θ� root �� �ƴҶ�  , root �� -1 �̴�
	if (parent >= 0) {
		matParent = mBones[parent]->transform;
	}
	bone->transform = bone->transform * matParent;
	mBones.push_back(bone);

	//���� �ѹ��� ���� �Ѱ��༭ meshdata �� �о�´� .
	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; ++i) {
		ReadModelData(node->mChildren[i], mBones.size(), index);
	}

}

void Converter::ReadSkinData()
{
	for (uint32 i = 0; i < mScene->mNumMeshes; ++i) {
		aiMesh* srcMesh = mScene->mMeshes[i];

		if (srcMesh->HasBones() == false) {
			continue;
		}

		//mmeshes �� i 
		shared_ptr<asMesh> mesh = mMeshes[i];

		//�� ���ؽ� ���� asBoneweight �� �����Ѵ�.
		vector<asBoneWeight> tempVertexBoneWeights;
		tempVertexBoneWeights.resize(mesh->vertices.size());


		for (UINT b = 0; b < srcMesh->mNumBones; b++) {
			aiBone* srcMeshBone = srcMesh->mBones[b];
			uint32 boneIndex = GetBoneIndex(srcMeshBone->mName.C_Str());
			//srcMeshbone �� idx �� �������� 

			for (uint32 w = 0; w < srcMeshBone->mNumWeights; w++) {
				uint32 index = srcMeshBone->mWeights[w].mVertexId;
				float weight = srcMeshBone->mWeights[w].mWeight;

				tempVertexBoneWeights[index].AddWeights(boneIndex, weight);
			}

		}

		for (UINT v = 0; v < tempVertexBoneWeights.size(); ++v) {
			tempVertexBoneWeights[v].Normalize();

			asBlendWeight blendWeight = tempVertexBoneWeights[v].GetBlendWeight();
			mesh->vertices[v].blendIndices = blendWeight.indices;
			mesh->vertices[v].blendWeights = blendWeight.weights;
		}

	
	}


}

void Converter::ReadMeshData(aiNode* node, int32 bone)
{
	//bone �̶�  �� ����� �Ž��� �ش��ϴ� �� �ѹ��Դϴ�.(index)

	//���ǻ���!! 
	// �ϳ��� ��忡�� �Ž��� �������� ���� �� �ִٴ� �� !!
	if (node->mNumMeshes < 1) {
		return;
	}

	shared_ptr<asMesh> mesh = make_shared<asMesh>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;


	//�������� �Ž��� �־ �ϳ��� vector<Vertex> , vector<index> �� ������

	for (uint32 i = 0; i < node->mNumMeshes; ++i) {
			
		uint32 index = node->mMeshes[i];
		aiMesh* srcMesh = mScene->mMeshes[index];

		const aiMaterial* srcMaterial = mScene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = srcMaterial->GetName().C_Str();

		//�������� �Ž��� �ϳ��� ���ͷ� �������� startVertex �ѹ��� �˾ƾ���
		const uint32 startVertex = mesh->vertices.size();


		for (uint32 j = 0; j < srcMesh->mNumVertices; ++j) {
			VertexType vertex;
			
			//vertex �� ���� �� �ִ� �����ʹ� �־��ش�.
			memcpy(&vertex.position, &srcMesh->mVertices[j], sizeof(Vec3));
			
			if (srcMesh->HasTextureCoords(0)) {
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][j], sizeof(Vec2));
			}

			if (srcMesh->HasNormals()) {
				memcpy(&vertex.normal, &srcMesh->mNormals[j], sizeof(Vec3));
			}
			
			if (srcMesh->HasTangentsAndBitangents()) {
				memcpy(&vertex.tangent, &srcMesh->mTangents[j], sizeof(Vec3));

			}
			mesh->vertices.push_back(vertex);
		}

		for (uint32 j = 0; j < srcMesh->mNumFaces; ++j) {
			aiFace& face = srcMesh->mFaces[j];

			for (uint32 k = 0; k < face.mNumIndices; ++k) {
				mesh->indices.push_back(face.mIndices[k] + startVertex);
			}
		}
	
	}


	mMeshes.push_back(mesh);
}



void Converter::ReadMaterialData()
{
	//Scene �� �ִ� ��� material �� ��ȯ
	for (UINT i = 0; i < mScene->mNumMaterials; ++i) {
		aiMaterial* srcMaterial = mScene->mMaterials[i];
		
		//aiMaterial -> asMaterial �� ��ȯ
		shared_ptr<asMaterial> material = make_shared<asMaterial>();
		//GetName �Լ��� aiString �� ��ȯ
		material->name = srcMaterial->GetName().C_Str();


		aiColor3D color;
		//Ambient �޾ƿͼ� material->ambient �� �ֱ� 
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = Color(color.r, color.g, color.b, 1.f);
	
		// Diffuse
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = Color(color.r, color.g, color.b, 1.f);

		// Specular
		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = Color(color.r, color.g, color.b, 1.f);
		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w);

		// Emissive
		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->emissive = Color(color.r, color.g, color.b, 1.0f);

		//material �� materialdesc ��ȯ �� ;


		aiString file;
		//srcMaterial �� �ִ� texture ��ġ�� file �� �޾ƿ´�. 

		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->diffuseFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->specularFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->normalFile = file.C_Str();


		mMaterials.push_back(material);

	
	}
}




/// <summary>
/// bone���� ��[1] ��[2]...
/// mesh���� mesh[1] mesh[2]...
/// </summary>
/// <param name="finalPath"></param>
void Converter::WriteModelFile(wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	filesystem::create_directory(path.parent_path());


	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);


	//1.bone 2.mesh

	file->Write<uint32>(mBones.size());
	for (shared_ptr<asBone> bone : mBones) {
		//�ε��� ���� ��Ʈ Ʈ������
		file->Write<int32>(bone->index);
		file->Write<string>(bone->name);
		file->Write<int32>(bone->parent);
		file->Write<Matrix>(bone->transform);
	}

	file->Write<uint32>(mMeshes.size());
	for (shared_ptr<asMesh> mesh : mMeshes) {
		file->Write<string>(mesh->name);
		file->Write<int32>(mesh->boneIndex);
		file->Write<string>(mesh->materialName);

		file->Write<uint32>(mesh->vertices.size());
		file->Write(&mesh->vertices[0], sizeof(VertexType) * mesh->vertices.size());

		file->Write<uint32>(mesh->indices.size());
		file->Write(&mesh->indices[0], sizeof(uint32) * mesh->indices.size());
	}



}

void Converter::WriteMaterialData(wstring finalPath)
{
	auto path = filesystem::path(finalPath);
	
	//������ ������ �������ϱ⿡
	//������ ����
	filesystem::create_directory(path.parent_path());
	

	string folder_path = path.parent_path().string();

	//�������� Material ���� xml �� ���� �����Ѵ�. 
	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

		//��Ŭ�����̼� ���� ��ť��Ʈ�� ���̱� 
	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	for (shared_ptr<asMaterial> material : mMaterials) {
		

		tinyxml2::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		//�� ��忡 ���� �����͸� ���̴°��� 

		tinyxml2::XMLElement* element = nullptr;

		element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder_path, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder_path, material->specularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder_path, material->normalFile).c_str());
		node->LinkEndChild(element);


		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->emissive.x);
		element->SetAttribute("G", material->emissive.y);
		element->SetAttribute("B", material->emissive.z);
		element->SetAttribute("A", material->emissive.w);
		node->LinkEndChild(element);
	}
	//wstring->string ->const char*
	document->SaveFile(Utils::ToString(finalPath).c_str());

	//xml �� 



}

std::string Converter::WriteTexture(string saveFolder, string file)
{
	string fileName = filesystem::path(file).filename().string();
	string folderName = filesystem::path(saveFolder).filename().string();



	const aiTexture* srcTexture = mScene->GetEmbeddedTexture(file.c_str());
	//�Ӻ���� �ؽ��İ� �ֳ���?
	//������ ���ڴ� ..
	if (srcTexture)
	{
		string pathStr = (filesystem::path(saveFolder) / fileName).string();

		if (srcTexture->mHeight == 0)
		{
			shared_ptr<FileUtils> file = make_shared<FileUtils>();
			file->Open(Utils::ToWString(pathStr), FileMode::Write);
			file->Write(srcTexture->pcData, srcTexture->mWidth);
		}
		else
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = srcTexture->mWidth;
			desc.Height = srcTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = srcTexture->pcData;

			ComPtr<ID3D11Texture2D> texture;
			HRESULT hr = DEVICE->CreateTexture2D(&desc, &subResource, texture.GetAddressOf());
			CHECK(hr);

			DirectX::ScratchImage img;
			::CaptureTexture(DEVICE.Get(), CONTEXT.Get(), texture.Get(), img);

			// Save To File
			hr = DirectX::SaveToDDSFile(*img.GetImages(), DirectX::DDS_FLAGS_NONE, Utils::ToWString(fileName).c_str());
			CHECK(hr);
		}
	}
	else
	{
		//���� ��쿡 
		string originStr = (filesystem::path(mAssetBasePath) / folderName / file).string();
		Utils::Replace(originStr, "\\", "/");

		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		Utils::Replace(pathStr, "\\", "/");

		//���Ϻ��� 
		::CopyFileA(originStr.c_str(), pathStr.c_str(), false);
	}

	return fileName;

}

std::shared_ptr<asAnimation> Converter::ReadAnimationData(aiAnimation* srcAnimation)
{
	shared_ptr<asAnimation> animation = make_shared<asAnimation>();
	animation->name = srcAnimation->mName.C_Str();
	animation->frameRate = (float)srcAnimation->mTicksPerSecond;
	animation->frameCount = (uint32)srcAnimation->mDuration + 1;

	map<string, shared_ptr<asAnimationNode>> cacheAnimNodes;

	for (uint32 i = 0; i < srcAnimation->mNumChannels; i++)
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		// �ִϸ��̼� ��� ������ �Ľ�
		shared_ptr<asAnimationNode> node = ParseAnimationNode(animation, srcNode);

		// ���� ã�� ��� �߿� ���� �� �ð����� �ִϸ��̼� �ð� ����
		animation->duration = max(animation->duration, node->keyframe.back().time);

		cacheAnimNodes[srcNode->mNodeName.C_Str()] = node;
	}

	ReadKeyframeData(animation, mScene->mRootNode, cacheAnimNodes);

	return animation;
}

std::shared_ptr<asAnimationNode> Converter::ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode)
{
	std::shared_ptr<asAnimationNode> node = make_shared<asAnimationNode>();
	node->name = srcNode->mNodeName;

	uint32 keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys), srcNode->mNumRotationKeys);

	for (uint32 k = 0; k < keyCount; k++)
	{
		asKeyframeData frameData;

		bool found = false;
		uint32 t = node->keyframe.size();

		// Position
		if (::fabsf((float)srcNode->mPositionKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mPositionKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.translation, sizeof(Vec3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		// Rotation
		if (::fabsf((float)srcNode->mRotationKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiQuatKey key = srcNode->mRotationKeys[k];
			frameData.time = (float)key.mTime;

			frameData.rotation.x = key.mValue.x;
			frameData.rotation.y = key.mValue.y;
			frameData.rotation.z = key.mValue.z;
			frameData.rotation.w = key.mValue.w;

			found = true;
		}

		// Scale
		if (::fabsf((float)srcNode->mScalingKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mScalingKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.scale, sizeof(Vec3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		if (found == true)
			node->keyframe.push_back(frameData);
	}

	// Keyframe �÷��ֱ�
	if (node->keyframe.size() < animation->frameCount)
	{
		uint32 count = animation->frameCount - node->keyframe.size();
		asKeyframeData keyFrame = node->keyframe.back();

		for (uint32 n = 0; n < count; n++)
			node->keyframe.push_back(keyFrame);
	}

	return node;
}

void Converter::ReadKeyframeData(shared_ptr<asAnimation> animation, aiNode* node, map<string, shared_ptr<asAnimationNode>>& cache)
{
	shared_ptr<asKeyframe> keyframe = make_shared<asKeyframe>();
	keyframe->boneName = node->mName.C_Str();

	shared_ptr<asAnimationNode> findNode = cache[node->mName.C_Str()];

	for (uint32 i = 0; i < animation->frameCount; i++)
	{
		asKeyframeData frameData;

		if (findNode == nullptr)
		{
			Matrix transform(node->mTransformation[0]);
			transform = transform.Transpose();
			frameData.time = (float)i;
			transform.Decompose(OUT frameData.scale, OUT frameData.rotation, OUT frameData.translation);
		}
		else
		{
			frameData = findNode->keyframe[i];
		}

		keyframe->transforms.push_back(frameData);
	}

	// �ִϸ��̼� Ű������ ä���
	animation->keyframes.push_back(keyframe);

	for (uint32 i = 0; i < node->mNumChildren; i++)
		ReadKeyframeData(animation, node->mChildren[i], cache);
}

void Converter::WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	// ������ ������ �����.
	filesystem::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	file->Write<string>(animation->name);
	file->Write<float>(animation->duration);
	file->Write<float>(animation->frameRate);
	file->Write<uint32>(animation->frameCount);

	file->Write<uint32>(animation->keyframes.size());

	for (shared_ptr<asKeyframe> keyframe : animation->keyframes)
	{
		file->Write<string>(keyframe->boneName);

		file->Write<uint32>(keyframe->transforms.size());
		file->Write(&keyframe->transforms[0], sizeof(asKeyframeData) * keyframe->transforms.size());
	}
}

uint32 Converter::GetBoneIndex(const string& name)
{

	for (auto bone : mBones) {
	
		if (bone->name == name) {
			return bone->index;
		}
	}

	//���࿡ �׷� ���� ������ 0 ����

	return 0;


}
