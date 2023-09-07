#pragma once

namespace Vertex {

	struct VertexFull
	{
		VertexFull() {}
		VertexFull(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv) {}
		VertexFull(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: Position(px, py, pz), Normal(nx, ny, nz),
			TangentU(tx, ty, tz), TexC(u, v) {}

		XMFLOAT3 Position;
		XMFLOAT2 TexC;
		XMFLOAT3 Normal;
		XMFLOAT3 TangentU;
		
	};



	struct VertexData
	{

		
		Vec3 position = { 0, 0, 0 };
	};

	struct VertexTextureData
	{
		
		//VertexTextureData(const VertexFull& rhs) 
		//	:position(rhs.Position),uv(rhs.TexC)
		//{}
		
		Vec3 position = { 0, 0, 0 };
		Vec2 uv = { 0, 0 };
	};

	struct VertexColorData
	{
		Vec3 position = { 0, 0, 0 };
		Color color = { 0, 0, 0, 0 };
	};

	struct VertexTextureNormalData
	{
		Vec3 position = { 0, 0, 0 };
		Vec2 uv = { 0, 0 };
		Vec3 normal = { 0, 0, 0 };
	};

	struct VertexTextureNormalTangentData
	{
		Vec3 position = { 0, 0, 0 };
		Vec2 uv = { 0, 0 };
		Vec3 normal = { 0, 0, 0 };
		Vec3 tangent = { 0,0,0 };
	};


	struct VertexTextureNormalTangentBlendData {
		
		Vec3 position = { 0, 0, 0 };
		Vec2 uv = { 0, 0 };
		Vec3 normal = { 0, 0, 0 };
		Vec3 tangent = { 0, 0, 0 };
		Vec4 blendIndices = { 0, 0, 0, 0 };
		Vec4 blendWeights = { 0, 0, 0, 0 };
	};



	struct VertexPosSize {
		Vec3 position = { 0,0,0 };
		Vec2 size = { 0,0 };

	};

	struct VertexTerrain
	{
		Vec3 Pos;
		Vec2 Tex;
		Vec2 BoundsY;
	};

	//첨에 Vec4 안쓰니까 constructgswith 할때 오류가 나네

	struct Particle {
		Vec4 InitialPos;
		Vec3 InitialVel;
		Vec2 Size;
		float Age;
		uint32 Type;
	};
};

using namespace Vertex;
using ModelVertexType = VertexTextureNormalTangentBlendData;