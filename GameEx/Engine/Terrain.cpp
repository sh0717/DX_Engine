#include "pch.h"
#include "Terrain.h"
#include <fstream>

Terrain::Terrain()
	:Component(eComponentType::Terrain)
{

}



Terrain::~Terrain()
{

}

void Terrain::Update()
{

}

void Terrain::InitTerrain(float hScale, float cellspace, uint32 hmapwidth, uint32 hmapheight,wstring heightmapfiles)
{
	heightMapFilename = heightmapfiles;
	heightScale = hScale;
	cellSpacing = cellspace;
	heightmapHeight = hmapheight;
	heightmapWidth = hmapwidth;


	mNumPatchVertRows = ((heightmapHeight - 1) / cellsPerPath) + 1;
	mNumPatchVertCols = ((heightmapWidth - 1) / cellsPerPath) + 1;

	mNumPatchVertices = mNumPatchVertRows * mNumPatchVertCols;
	mNumPatch = (mNumPatchVertRows - 1) * (mNumPatchVertCols - 1);

	//make the _heightmap vector
	LoadHeightMap();
	Smooth();
	
	//this is calculating boundY

	//

	//after the _heightmap is made 
	BuildVBandIB();
	SetMesh();


	BuildHeightMapSRV();


}

float Terrain::GetWidth() const
{
	return (heightmapWidth - 1) * cellSpacing;
}

float Terrain::GetHeight() const
{
	return (heightmapHeight - 1) * cellSpacing;
}

void Terrain::BuildVBandIB()
{
	
	std::vector<Vertex::VertexTerrain> patchVertices(mNumPatchVertRows * mNumPatchVertCols);

	float halfWidth = 0.5f * GetWidth();
	float halfDepth = 0.5f * GetHeight();

	//패치당 길이 
	float patchWidth = GetWidth() / (mNumPatchVertCols - 1);
	float patchDepth = GetHeight() / (mNumPatchVertRows - 1);
	float du = 1.0f / (mNumPatchVertCols - 1);
	float dv = 1.0f / (mNumPatchVertRows - 1);

	for (uint32 i = 0; i < mNumPatchVertRows; ++i)
	{
		float z = halfDepth - i * patchDepth;
		for (uint32 j = 0; j < mNumPatchVertCols; ++j)
		{
			float x = -halfWidth + j * patchWidth;

			patchVertices[i * mNumPatchVertCols + j].Pos =Vec3(x, 0.0f, z);

			// Stretch texture over grid.
			patchVertices[i * mNumPatchVertCols + j].Tex.x = j * du;
			patchVertices[i * mNumPatchVertCols + j].Tex.y = i * dv;
		}
	}

	mVB = make_shared<VertexBuffer>();
	mVB->CreateBuffer(patchVertices);


	std::vector<UINT> indices(mNumPatch * 4); // 4 indices per quad face

	// Iterate over each quad and compute indices.
	int32 k = 0;
	for (uint32 i = 0; i < mNumPatchVertRows - 1; ++i)
	{
		for (uint32 j = 0; j < mNumPatchVertCols - 1; ++j)
		{
			// Top row of 2x2 quad patch
			indices[k] = i * mNumPatchVertCols + j;
			indices[k + 1] = i * mNumPatchVertCols + j + 1;

			// Bottom row of 2x2 quad patch
			indices[k + 2] = (i + 1) * mNumPatchVertCols + j;
			indices[k + 3] = (i + 1) * mNumPatchVertCols + j + 1;

			k += 4; // next quad
		}
	}

	mIB = make_shared<IndexBuffer>();
	mIB->CreateBuffer(indices);

}

void Terrain::SetMesh()
{
	auto object = GetGameObject();

	auto meshRenderer = object->GetMeshRenderer();
	if (meshRenderer == nullptr) {
		object->AddComponent(make_shared<MeshRenderer>());
	}

	auto mesh = make_shared<Mesh>();
	mesh->SetVB(mVB);
	mesh->SetIB(mIB);


	object->GetMeshRenderer()->SetMesh(mesh);
	

}

void Terrain::LoadHeightMap()
{
	vector<unsigned char> in(heightmapWidth * heightmapHeight);
	std::ifstream inFile;
	//i will open this file as binary
	inFile.open(heightMapFilename.c_str(), std::ios_base::binary);


	if (inFile) {
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	
	}

	_heightmap.resize(heightmapWidth * heightmapHeight, 0);

	for (uint32 i = 0; i < heightmapWidth * heightmapHeight; ++i)
	{
		_heightmap[i] = (in[i] / 255.0f) * heightScale;
	}
	

}

void Terrain::Smooth()
{
	std::vector<float> dest(_heightmap.size());

	for (uint32 i = 0; i <heightmapHeight; ++i)
	{
		for (uint32 j = 0; j < heightmapWidth; ++j)
		{
			dest[i * heightmapWidth + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	_heightmap = dest;
}


//오일러 스무스?
float Terrain::Average(int32 i, int32 j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	// Use int to allow negatives.  If we use UINT, @ i=0, m=i-1=UINT_MAX
	// and no iterations of the outer for loop occur.
	for (int32 m = i - 1; m <= i + 1; ++m)
	{
		for (int32 n = j - 1; n <= j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				avg += _heightmap[m * heightmapWidth + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

bool Terrain::InBounds(int32 i, int32 j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < (int32)heightmapHeight &&
		j >= 0 && j < (int32)heightmapWidth;
}

void Terrain::BuildHeightMapSRV()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = heightmapWidth;
	texDesc.Height = heightmapHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// HALF is defined in xnamath.h, for storing 16-bit float.
	std::vector<uint16> hmap(_heightmap.size());
	std::transform(_heightmap.begin(), _heightmap.end(), hmap.begin(), MathHelper::ConvertFloatToHalf);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
	data.SysMemPitch = heightmapWidth * sizeof(uint16);
	data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	DEVICE->CreateTexture2D(&texDesc, &data, &hmapTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	DEVICE->CreateShaderResourceView(hmapTex, &srvDesc, _heightMapSRV.GetAddressOf());




}

std::shared_ptr<Texture> Terrain::GetHieghtMapTextureFromTerrain()
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->SetSRV(_heightMapSRV);
	return texture;
}
