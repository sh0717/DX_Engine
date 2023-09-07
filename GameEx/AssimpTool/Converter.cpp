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
	//asset 이 존재하는 파일 경로


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
	//모델데이터를 저장할 경로 == finalPath
	wstring finalPath = mModelBasePath + savePath + L".mesh";
	

	//여기서 재귀적으로 읽어서 vector에 채운다. 
	ReadModelData(mScene->mRootNode, -1, -1);
	
	//mmeshes , mBones 채운 이후

	//그 메쉬들의 블렌드 데이터를 채워준다.
	ReadSkinData();

	
	WriteModelFile(finalPath);

}

void Converter::ExportMaterialData(wstring savePath)
{
	wstring finalPath = mTextureBasePath + savePath + L".xml";

	//scene 에서 material 읽어서 mMaterials 에 채운다음
	ReadMaterialData();

	//finalPath 에  xml 저장해줌
	WriteMaterialData(finalPath);


}

void Converter::ExportAnimationData(wstring savePath, uint32 index /*= 0*/)
{
	//에니메이션 데이터는 확장자 .clip
	wstring finalPath = mModelBasePath + savePath + L".clip";
	assert(index < mScene->mNumAnimations);

	shared_ptr<asAnimation> animation = ReadAnimationData(mScene->mAnimations[index]);
	WriteAnimationData(animation, finalPath);


}

void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)
{
	//bone 마다 idx 가 존재  parent 도 존재 
	
	shared_ptr<asBone> bone = make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();


	Matrix transform(node->mTransformation[0]);
	//이거는 relative transformation
	bone->transform = transform.Transpose();


	Matrix matParent = Matrix::Identity;
	//부모가 root 가 아닐때  , root 는 -1 이다
	if (parent >= 0) {
		matParent = mBones[parent]->transform;
	}
	bone->transform = bone->transform * matParent;
	mBones.push_back(bone);

	//본의 넘버를 같이 넘겨줘서 meshdata 를 읽어온다 .
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

		//mmeshes 의 i 
		shared_ptr<asMesh> mesh = mMeshes[i];

		//각 버텍스 마다 asBoneweight 가 존재한다.
		vector<asBoneWeight> tempVertexBoneWeights;
		tempVertexBoneWeights.resize(mesh->vertices.size());


		for (UINT b = 0; b < srcMesh->mNumBones; b++) {
			aiBone* srcMeshBone = srcMesh->mBones[b];
			uint32 boneIndex = GetBoneIndex(srcMeshBone->mName.C_Str());
			//srcMeshbone 의 idx 를 가져오고 

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
	//bone 이란  이 노드의 매쉬에 해당하는 본 넘버입니다.(index)

	//주의사항!! 
	// 하나의 노드에는 매쉬가 여러개도 있을 수 있다는 점 !!
	if (node->mNumMeshes < 1) {
		return;
	}

	shared_ptr<asMesh> mesh = make_shared<asMesh>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;


	//여러개의 매쉬가 있어도 하나의 vector<Vertex> , vector<index> 로 저장해

	for (uint32 i = 0; i < node->mNumMeshes; ++i) {
			
		uint32 index = node->mMeshes[i];
		aiMesh* srcMesh = mScene->mMeshes[index];

		const aiMaterial* srcMaterial = mScene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = srcMaterial->GetName().C_Str();

		//여러개의 매쉬를 하나의 벡터로 넣으려면 startVertex 넘버를 알아야해
		const uint32 startVertex = mesh->vertices.size();


		for (uint32 j = 0; j < srcMesh->mNumVertices; ++j) {
			VertexType vertex;
			
			//vertex 에 넣을 수 있는 데이터는 넣어준다.
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
	//Scene 에 있는 모든 material 을 변환
	for (UINT i = 0; i < mScene->mNumMaterials; ++i) {
		aiMaterial* srcMaterial = mScene->mMaterials[i];
		
		//aiMaterial -> asMaterial 로 변환
		shared_ptr<asMaterial> material = make_shared<asMaterial>();
		//GetName 함수가 aiString 을 반환
		material->name = srcMaterial->GetName().C_Str();


		aiColor3D color;
		//Ambient 받아와서 material->ambient 에 넣기 
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

		//material 의 materialdesc 변환 끝 ;


		aiString file;
		//srcMaterial 에 있는 texture 위치를 file 로 받아온다. 

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
/// bone개수 본[1] 본[2]...
/// mesh개수 mesh[1] mesh[2]...
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
		//인덱스 네임 페어런트 트랜스폼
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
	
	//폴더가 없으면 만들어야하기에
	//있으면 말고
	filesystem::create_directory(path.parent_path());
	

	string folder_path = path.parent_path().string();

	//이제부터 Material 관련 xml 을 만들어서 저장한다. 
	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

		//데클러레이션 만들어서 다큐먼트에 붙이기 
	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	for (shared_ptr<asMaterial> material : mMaterials) {
		

		tinyxml2::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		//각 노드에 인제 데이터를 붙이는거지 

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

	//xml 끝 



}

std::string Converter::WriteTexture(string saveFolder, string file)
{
	string fileName = filesystem::path(file).filename().string();
	string folderName = filesystem::path(saveFolder).filename().string();



	const aiTexture* srcTexture = mScene->GetEmbeddedTexture(file.c_str());
	//임베디드 텍스쳐가 있나요?
	//없으면 좋겠다 ..
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
		//없을 경우에 
		string originStr = (filesystem::path(mAssetBasePath) / folderName / file).string();
		Utils::Replace(originStr, "\\", "/");

		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		Utils::Replace(pathStr, "\\", "/");

		//파일복사 
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

		// 애니메이션 노드 데이터 파싱
		shared_ptr<asAnimationNode> node = ParseAnimationNode(animation, srcNode);

		// 현재 찾은 노드 중에 제일 긴 시간으로 애니메이션 시간 갱신
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

	// Keyframe 늘려주기
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

	// 애니메이션 키프레임 채우기
	animation->keyframes.push_back(keyframe);

	for (uint32 i = 0; i < node->mNumChildren; i++)
		ReadKeyframeData(animation, node->mChildren[i], cache);
}

void Converter::WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	// 폴더가 없으면 만든다.
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

	//만약에 그런 본이 없으면 0 린턴

	return 0;


}
