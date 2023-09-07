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
	/// 벡터
	/// </summary>
	vector<VertexType> vertices;
	vector<uint32> indices;


	//이 메쉬에 해당하는 본과 머티리얼이요
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

		
	//idx 번의 자리에 (boneidx, weight)
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
/// (bone weight) 정보를 저장해주는 저장소 
/// 여기에 일단 넣은 다음에 blendweight, blendindex 를 만들어준다. 
/// </summary>
struct asBoneWeight {
	//{본인덱스, 웨이트} vector
	std::vector<pair<int32, float>> boneWieghts;


	//주의사항!!  bone, 웨이트 정보가 4개만 처리할거지만
	//input은 4개 이상일 수도 있다 .
	//그래서 가장 weight 가 큰 4개만 가져와서 할거다
	//정렬유지!
	void AddWeights(uint32 boneIndex, float weight) {
		if (weight <= 0.f) {
			return;
		}

		auto findit = std::find_if(boneWieghts.begin(), boneWieghts.end(),

			[weight](const pair<int32, float>& p) {return weight > p.second; });
		
		boneWieghts.insert(findit, make_pair(boneIndex, weight));
	}

	//4개만
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
//각 본당 transfomr 정보들(애니메이션에서)
struct asKeyframe {
	string boneName;
	std::vector<asKeyframeData> transforms;
};

//askeyframe 이 모인 데이터 -> 모든 본들의 keyframe 이 포함
struct asAnimation {
	//애니메이션의 네임
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