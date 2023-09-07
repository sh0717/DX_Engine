#pragma once
#include "Geometry.h"
#include "VertexData.h"

using namespace Vertex;
class GeometryHelper
{
public:
	static void CreateRectangle(shared_ptr<Geometry<Vertex::VertexColorData>> geometry, Color color);

	static void CreateRectangle(shared_ptr<Geometry<Vertex::VertexFull>> geometry);




	static void CreateSphere(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float radius, UINT sliceCount, UINT stackCount);
	static void CreateCube(shared_ptr<Geometry<Vertex::VertexFull>>geometry, float width, float height, float depth);
	static void CreateGrid(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float width, float depth, UINT m, UINT n, UINT endU, UINT endV );
	static void CreateWave(shared_ptr<Geometry<Vertex::VertexFull>> geometry, UINT m, UINT n);
	static void CreateCylinder(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount);
	static void CreateFullCylinder(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount);

private:
	static void BuildCylinderTopCap(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount);
	static void BuildCylinderBottomCap(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount);
};

