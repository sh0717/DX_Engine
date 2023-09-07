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
	//���� ������ �о�   mBones ä���
	void ReadModelData(aiNode* node, int32 index, int32 parent);

	//�� ��Ų������ �о �Ž��� vertex�� boneidx, weight ä���
	void ReadSkinData();


	//�޽������͸� �о vector<asMEsh> �� ���� mMeshes ä���
	void ReadMeshData(aiNode* node, int32 bone);

	//Scene ���� material �о mMaterials �� ä��� �Լ� 
	void ReadMaterialData();






	//final path �� ���̳� �Ž� ���� ������ binary �� ���� 
	void WriteModelFile(wstring finalPath);

	//mMaterials �� �ִ� asMaterial ���� ���ؼ� xml �̶� texture ����� �Լ�
	void WriteMaterialData(wstring finalPath);


	//Resources/Textures �ʿ� texture file�� ������ָ鼭 filename�� return �ϴ� �Լ�
	string WriteTexture(string saveFolder, string file);



	///�ִϸ��̼� ���� read write


	shared_ptr<asAnimation> ReadAnimationData(aiAnimation* srcAnimation);
	shared_ptr<asAnimationNode> ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode);
	void ReadKeyframeData(shared_ptr<asAnimation> animation, aiNode* node, map<string, shared_ptr<asAnimationNode>>& cache);
	void WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath);


	uint32 GetBoneIndex(const string& name);

private:

	/// <summary>
	/// Asset �о���� ������ 
	/// �� �ؽ��� �����ϴ� ������~
	//Model -> mesh and bone 
	//Texture -> Material and texture 
	/// </summary>
	wstring mAssetBasePath = L"../Resources/Assets/";
	wstring mModelBasePath = L"../Resources/Models/";
	wstring mTextureBasePath = L"../Resources/Textures/";



	shared_ptr<Assimp::Importer> mImporter;
	const aiScene* mScene;

	/// <summary>
	/// scene ���� ������ �����͵� �����ϴ� ����!! �޸𸮻�����!!
	/// </summary>
	vector<shared_ptr<asBone>> mBones;
	vector<shared_ptr<asMesh>> mMeshes;
	vector<shared_ptr<asMaterial>> mMaterials;

};

