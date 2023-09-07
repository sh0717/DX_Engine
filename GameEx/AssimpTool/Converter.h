#pragma once
#include "AsTypes.h"


class Converter
{

public:
	Converter();
	~Converter();
public:

	//Make Scene from .fbx file 
	void ReadAssetFile(wstring file);

	//from data -> refined asset file to savePath 
	void ExportModelData(wstring savePath);
	void ExportMaterialData(wstring savePath);
	void ExportAnimationData(wstring savePath, uint32 index = 0);
private:
	//모델의 본들을 읽어   mBones 채우기
	void ReadModelData(aiNode* node, int32 index, int32 parent);

	//음 스킨데이터 읽어서 매쉬의 vertex에 boneidx, weight 채우기
	void ReadSkinData();


	//메쉬데이터를 읽어서 vector<asMEsh> 에 저장 mMeshes 채우기
	void ReadMeshData(aiNode* node, int32 bone);

	//Scene 에서 material 읽어서 mMaterials 에 채우는 함수 
	void ReadMaterialData();






	//final path 에 본이나 매쉬 관련 정보를 binary 로 저장 
	void WriteModelFile(wstring finalPath);

	//mMaterials 에 있는 asMaterial 들을 통해서 xml 이랑 texture 만드는 함수
	void WriteMaterialData(wstring finalPath);


	//Resources/Textures 쪽에 texture file을 만들어주면서 filename을 return 하는 함수
	string WriteTexture(string saveFolder, string file);



	///애니메이션 관련 read write


	shared_ptr<asAnimation> ReadAnimationData(aiAnimation* srcAnimation);
	shared_ptr<asAnimationNode> ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode);
	void ReadKeyframeData(shared_ptr<asAnimation> animation, aiNode* node, map<string, shared_ptr<asAnimationNode>>& cache);
	void WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath);


	uint32 GetBoneIndex(const string& name);

private:

	/// <summary>
	/// Asset 읽어오는 폴더와 
	/// 모델 텍스텨 저장하는 폴더요~
	//Model -> mesh and bone 
	//Texture -> Material and texture 
	/// </summary>
	wstring mAssetBasePath = L"../Resources/Assets/";
	wstring mModelBasePath = L"../Resources/Models/";
	wstring mTextureBasePath = L"../Resources/Textures/";



	shared_ptr<Assimp::Importer> mImporter;
	const aiScene* mScene;

	/// <summary>
	/// scene 에서 추출한 데이터들 저장하는 벡터!! 메모리상에저장!!
	/// </summary>
	vector<shared_ptr<asBone>> mBones;
	vector<shared_ptr<asMesh>> mMeshes;
	vector<shared_ptr<asMaterial>> mMaterials;

};

