#pragma once
class Terrain:public Component
{
public:
	Terrain();
	virtual ~Terrain();

	virtual void Update();



	void InitTerrain(float hScale, float cellspace, uint32 hmapwidth, uint32 hmapheight,wstring heightmapfiles);

	//terrain 의 총 가로길이
	float GetWidth() const;

	//terrain 의 총 세로길이
	float GetHeight() const;



private:
	void BuildVBandIB();
	void SetMesh();

	//load the data from raw file
	void LoadHeightMap();
	// smooth the data to make it realistic in _hieghtmap
	void Smooth();


	//helper function for Smooth
	//it is something like gaussian filter
	float Average(int32 i, int32 j);
	bool InBounds(int32 i, int32 j);




	void BuildHeightMapSRV();

private:
	//heightmap filename 
	std::wstring heightMapFilename;


	shared_ptr<VertexBuffer> mVB;
	shared_ptr<IndexBuffer> mIB;

	enum {cellsPerPath = 64};

	float heightScale;
	float cellSpacing;

	//height 맵이 가로몇 세로 몇인데요?
	uint32 heightmapWidth;
	uint32 heightmapHeight;


	uint32 mNumPatchVertRows;
	uint32 mNumPatchVertCols;
	uint32 mNumPatchVertices;
	uint32 mNumPatch;


	//store the data of "height.raw"
	std::vector<float> _heightmap;
	ComPtr<ID3D11ShaderResourceView> _heightMapSRV;
	///
public:
	shared_ptr<Texture> GetHieghtMapTextureFromTerrain();

};

