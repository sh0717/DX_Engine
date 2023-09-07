#pragma once

/// <summary>
/// /자 본이랑 메쉬 바이너리로 만들어놓은거 ModelBone  ModelMesh 로 읽어올거야!
/// </summary>
struct ModelBone
{
	wstring name;
	int32 index;
	int32 parentIndex;
	shared_ptr<ModelBone> parent; // Cache

	Matrix transform;
	vector<shared_ptr<ModelBone>> children; // Cache
};

struct ModelMesh
{
	//geometry -> vb ib 로 만들어준다. 
	void CreateBuffers();

	wstring name;

	// Mesh
	shared_ptr<Geometry<ModelVertexType>> geometry = make_shared<Geometry<ModelVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;

	// Material
	wstring materialName = L"";
	shared_ptr<Material> material; // Cache

	// Bones
	int32 boneIndex;
	shared_ptr<ModelBone> bone; // Cache;
};



struct ModelKeyframeData
{
	float time;
	Vec3 scale;
	Quaternion rotation;
	Vec3 translation;
};

struct ModelKeyframe
{
	wstring boneName;
	vector<ModelKeyframeData> transforms;
};

struct ModelAnimation
{
	//keyframes 중에서 name 에 맞는 keyframe 을 return 해준다.
	
	shared_ptr<ModelKeyframe> GetKeyframe(const wstring& name);

	wstring name;
	float duration = 0.f;
	float frameRate = 0.f;
	uint32 frameCount = 0;
	unordered_map<wstring, shared_ptr< ModelKeyframe>> keyframes;
};








class Model :public enable_shared_from_this<Model> {

public:
	Model();
	~Model();

	//xml 로부터 데이터를 읽어와서 material 을 만들어준다.
	void ReadMaterial(wstring filename);

	//binary file로부터 데이터를 읽어와서 model 생성 
	void ReadModel(wstring filename);

	//binary file 로부터 데이터를 읽어와서 mAnimations 에 추가
	void ReadAnimation(wstring filename);

	//about material 
	uint32 GetMaterialCount() { return (uint32)mMaterials.size(); }
	vector<shared_ptr<Material>>& GetMaterials() { return mMaterials; }
	shared_ptr<Material> GetMaterialByIndex(uint32 idx) { return mMaterials[idx]; }
	shared_ptr<Material> GetMaterialByName(const wstring& name);


	//about meshes 
	uint32 GetMeshCount() { return (uint32)mMeshes.size(); }
	vector<shared_ptr<ModelMesh>>& GetMeshes() { return mMeshes; }
	shared_ptr<ModelMesh> GetMeshByIndex(uint32 idx) { return mMeshes[idx]; }
	shared_ptr<ModelMesh> GetMeshByName(const wstring& name);

	//about bones 
	uint32 GetBoneCount() { return (uint32)mBones.size(); }
	vector<shared_ptr<ModelBone>>& GetBones() { return mBones; }
	shared_ptr<ModelBone> GetBoneByIndex(uint32 idx) { return mBones[idx]; }
	shared_ptr<ModelBone> GetBoneByName(const wstring& name);


	//about animation
	inline uint32 GetAnimationCount() { return (uint32)mAnimations.size(); }
	inline vector<shared_ptr<ModelAnimation>>& GetAnimations() { return mAnimations; }
	inline shared_ptr<ModelAnimation> GetAnimationByIndex(uint32 index) {
		return (index < 0 || index >= mAnimations.size()) ? nullptr : mAnimations[index];
	}
	shared_ptr<ModelAnimation> GetAnimationByName(wstring name);

	void BindCacheInfo();
private:
	wstring mModelBasePath = L"../Resources/Models/";
	wstring mTextureBasePath = L"../Resources/Textures/";


	shared_ptr<ModelBone> mRoot;
	vector<shared_ptr<Material>>  mMaterials;
	vector<shared_ptr<ModelBone>> mBones;
	vector<shared_ptr<ModelMesh>> mMeshes;
	

	std::vector<shared_ptr<ModelAnimation>> mAnimations;


};


