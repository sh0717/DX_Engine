#pragma once


using VertexType = ModelVertexType;

struct asBone {
	string name;
	int32 index = -1;
	int32 parent = -1;
	Matrix transform;
};

struct asMesh {

	string name;
	aiMesh* mesh;

	/// <summary>
	/// ����
	/// </summary>
	vector<VertexType> vertices;
	vector<uint32> indices;


	//�� �޽��� �ش��ϴ� ���� ��Ƽ�����̿�
	int32 boneIndex;
	string materialName;

};

struct asMaterial {


	string name;
	
	/// <summary>
	/// MaterialDesc
	/// </summary>
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;

	/// <summary>
	/// Texture
	/// </summary>
	string diffuseFile;
	string specularFile;
	string normalFile;



};


struct asBlendWeight {

		
	//idx ���� �ڸ��� (boneidx, weight)
	void Set(uint32 index, uint32 boneidx, float weight) {
	
		float i = (float)boneidx;
		float w = weight;

		switch (index)
		{
		case 0: indices.x = i; weights.x = w; break;
		case 1: indices.y = i; weights.y = w; break;
		case 2: indices.z = i; weights.z = w; break;
		case 3: indices.w = i; weights.w = w; break;
		}
	}

	Vec4 indices = Vec4(0.f);
	Vec4 weights = Vec4(0.f);

};





/// <summary>
/// (bone weight) ������ �������ִ� ����� 
/// ���⿡ �ϴ� ���� ������ blendweight, blendindex �� ������ش�. 
/// </summary>
struct asBoneWeight {
	//{���ε���, ����Ʈ} vector
	std::vector<pair<int32, float>> boneWieghts;


	//���ǻ���!!  bone, ����Ʈ ������ 4���� ó���Ұ�����
	//input�� 4�� �̻��� ���� �ִ� .
	//�׷��� ���� weight �� ū 4���� �����ͼ� �ҰŴ�
	//��������!
	void AddWeights(uint32 boneIndex, float weight) {
		if (weight <= 0.f) {
			return;
		}

		auto findit = std::find_if(boneWieghts.begin(), boneWieghts.end(),

			[weight](const pair<int32, float>& p) {return weight > p.second; });
		
		boneWieghts.insert(findit, make_pair(boneIndex, weight));
	}

	//4����
	asBlendWeight GetBlendWeight() {
		
		asBlendWeight blendWeights;
		for (uint32 i = 0; i < boneWieghts.size(); ++i) {
			if (i >= 4) {
				break;
			}
			blendWeights.Set(i, boneWieghts[i].first, boneWieghts[i].second);

		}
		return blendWeights;
	
	}


	void Normalize() {
	
		if (boneWieghts.size() >= 4) {
			boneWieghts.resize(4);
		}
		float totalWeight = 0.f;

		for (const auto& p : boneWieghts) {
			totalWeight += p.second;
		}

		float scale = 1.f / totalWeight;
		for (auto& item : boneWieghts) {
			item.second *= scale;
		}
	}

};


struct asKeyframeData {

	float time;
	Vec3 scale;
	Quaternion rotation;
	Vec3 translation;
};
//�� ���� transfomr ������(�ִϸ��̼ǿ���)
struct asKeyframe {
	string boneName;
	std::vector<asKeyframeData> transforms;
};

//askeyframe �� ���� ������ -> ��� ������ keyframe �� ����
struct asAnimation {
	//�ִϸ��̼��� ����
	string name;
	uint32 frameCount;
	float frameRate;
	float duration;
	std::vector<shared_ptr<asKeyframe>> keyframes;
};



struct asAnimationNode{
	aiString name;
	vector<asKeyframeData> keyframe;
};