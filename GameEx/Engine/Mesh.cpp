#include "pch.h"
#include "Mesh.h"

Mesh::Mesh()
	:ResourceBase(RESOURCE_TYPE::MESH)
{

}

Mesh::~Mesh()
{

}

void Mesh::CreateQuad()
{
	mGeo = make_shared<Geometry<VertexFull>>();
	GeometryHelper::CreateRectangle(mGeo);

	CreateBuffers();

}

void Mesh::CreateCube()
{
	mGeo = make_shared<Geometry<VertexFull>>();
	GeometryHelper::CreateCube(mGeo, 1, 1, 1);
	/*mGeoFull = make_shared<Geometry<VertexFull>>();

	GeometryHelper::CreateCube(mGeoFull, 1, 1, 1);

	auto& index = mGeoFull->GetIndices();
	auto& vertex = mGeoFull->GetVertices();

	mGeo->SetIndices(index);
	std::vector<VertexTextureNormalData> mVec;
	mVec.reserve(vertex.size());
	
	for (const auto& k : vertex) {
		VertexTextureNormalData v{ k.Position, k.TexC, k.Normal };
		mVec.push_back(v);
	}

	mGeo->SetVertices(mVec);*/
	CreateBuffers();

}

void Mesh::CreateGrid(int32 sizeX, int32 sizeZ)
{
	mGeo = make_shared<Geometry<VertexFull>>();
	GeometryHelper::CreateGrid(mGeo, 1, 1, sizeX, sizeZ, sizeX, sizeZ);

	/*mGeoFull = make_shared<Geometry<VertexFull>>();

	GeometryHelper::CreateGrid(mGeoFull, 1, 1, sizeX,sizeY,sizeX,sizeY);

	auto& index = mGeoFull->GetIndices();
	auto& vertex = mGeoFull->GetVertices();

	mGeo->SetIndices(index);
	std::vector<VertexTextureNormalData> mVec;
	mVec.reserve(vertex.size());

	for (auto k : vertex) {
		VertexTextureNormalData v{ k.Position, k.TexC, k.Normal };
		mVec.push_back(v);
	}
	mGeo->SetVertices(mVec);*/
	CreateBuffers();

}

void Mesh::CreateSphere()
{
	mGeo = make_shared<Geometry<VertexFull>>();
	GeometryHelper::CreateSphere(mGeo, 1, 50, 50);
	/*mGeoFull = make_shared<Geometry<VertexFull>>();

	GeometryHelper::CreateSphere(mGeoFull, 1, 50,50);

	auto& index = mGeoFull->GetIndices();
	auto& vertex = mGeoFull->GetVertices();

	mGeo->SetIndices(index);
	std::vector<VertexTextureNormalData> mVec;
	mVec.reserve(vertex.size());

	for (const auto& k : vertex) {
		VertexTextureNormalData v{ k.Position, k.TexC, k.Normal };
		mVec.push_back(v);
	}
	mGeo->SetVertices(mVec);*/
	CreateBuffers();

}
//
//void Mesh::CreateQuad()
//{
//	mGeo = make_shared<Geometry<VertexTextureNormalData>>();
//	mGeoFull = make_shared<Geometry<VertexFull>>();
//
//	GeometryHelper::CreateRectangle(mGeoFull);
//
//	auto& index = mGeoFull->GetIndices();
//	auto& vertex = mGeoFull->GetVertices();
//
//	mGeo->SetIndices(index);
//	std::vector<VertexTextureNormalData> mVec;
//	mVec.reserve(vertex.size());
//
//	for (auto k : vertex) {
//		VertexTextureNormalData v{ k.Position, k.TexC, k.Normal };
//		mVec.push_back(v);
//	}
//
//	CreateBuffers();
//}

void Mesh::CreateBuffers(bool CPUWrite , bool GpuWrite )
{
	mVB = make_shared<VertexBuffer>();
	mIB = make_shared<IndexBuffer>();

	mVB->CreateBuffer(mGeo->GetVertices(), 0,CPUWrite, GpuWrite);
	mIB->CreateBuffer(mGeo->GetIndices());
}
