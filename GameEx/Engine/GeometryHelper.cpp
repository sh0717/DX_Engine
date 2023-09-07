#include "pch.h"
#include "GeometryHelper.h"


using namespace Vertex;
void GeometryHelper::CreateRectangle(shared_ptr<Geometry<Vertex::VertexColorData>> geometry, Color color)
{
	vector<VertexColorData> vertices;
	vertices.resize(4);

	vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vertices[0].color = color;
	vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vertices[1].color = color;
	vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
	vertices[2].color = color;
	vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
	vertices[3].color = color;
	geometry->SetVertices(vertices);

	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(indices);
}

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexFull>> geometry)
{
	vector<VertexFull> vertices;
	vertices.resize(4);

	vertices[0].Position = Vec3(-0.5f, -0.5f, 0.f);
	vertices[0].TexC = Vec2(0.f, 1.f);
	vertices[0].Normal = Vec3(0.f, 0.f, -1.f);
	vertices[1].Position = Vec3(-0.5f, 0.5f, 0.f);
	vertices[1].TexC = Vec2(0.f, 0.f);
	vertices[1].Normal = Vec3(0.f, 0.f, -1.f);
	vertices[2].Position = Vec3(0.5f, -0.5f, 0.f);
	vertices[2].TexC = Vec2(1.f, 1.f);
	vertices[2].Normal = Vec3(0.f, 0.f, -1.f);
	vertices[3].Position = Vec3(0.5f, 0.5f, 0.f);
	vertices[3].TexC = Vec2(1.f, 0.f);
	vertices[3].Normal = Vec3(0.f, 0.f, -1.f);
	geometry->SetVertices(vertices);

	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(indices);
}

void GeometryHelper::CreateSphere(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float radius, UINT sliceCount, UINT stackCount)
{
	

	std::vector<VertexFull> Vertices;
	std::vector<uint32> Indices;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	VertexFull topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	VertexFull bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	Vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f * XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			VertexFull v;

			// spherical to cartesian
			v.Position.x = radius * sinf(phi) * cosf(theta);
			v.Position.y = radius * cosf(phi);
			v.Position.z = radius * sinf(phi) * sinf(theta);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius * sinf(phi) * sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius * sinf(phi) * cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.TangentU);
			XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Position);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / XM_2PI;
			v.TexC.y = phi / XM_PI;

			Vertices.push_back(v);
		}
	}

	Vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		Indices.push_back(0);
		Indices.push_back(i + 1);
		Indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			Indices.push_back(baseIndex + i * ringVertexCount + j);
			Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)Vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		Indices.push_back(southPoleIndex);
		Indices.push_back(baseIndex + i);
		Indices.push_back(baseIndex + i + 1);
	}

	geometry->SetIndices(Indices);
	geometry->SetVertices(Vertices);

}

void GeometryHelper::CreateCube(shared_ptr<Geometry<Vertex::VertexFull>>geometry, float width, float height, float depth)
{
	std::vector<VertexFull> v;
	v.resize(24);
	float w2 = 0.5f * width;
	float h2 = 0.5f * height;
	float d2 = 0.5f * depth;

	// Fill in the front face vertex data.
	v[0] = VertexFull(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = VertexFull(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = VertexFull(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = VertexFull(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = VertexFull(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = VertexFull(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = VertexFull(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = VertexFull(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = VertexFull(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = VertexFull(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = VertexFull(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = VertexFull(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = VertexFull(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = VertexFull(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = VertexFull(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = VertexFull(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = VertexFull(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = VertexFull(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = VertexFull(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = VertexFull(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = VertexFull(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = VertexFull(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = VertexFull(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = VertexFull(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	geometry->SetVertices(v);

	//
	// Create the indices.
	//

	uint32 i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	geometry->GetIndices().assign(&i[0], &i[36]);
}

void GeometryHelper::CreateGrid(shared_ptr<Geometry<VertexFull>> geometry, float width, float depth, UINT m, UINT n , UINT endU, UINT endV)
{

	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = (float)endU / (n - 1);
	float dv = (float)endV / (m - 1);

	std::vector<VertexFull> Vertices;
	std::vector<UINT> Indices;


	Vertices.resize(vertexCount);
	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			Vertices[i * n + j].Position = XMFLOAT3(x, 0.0f, z);
			Vertices[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			Vertices[i * n + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			Vertices[i * n + j].TexC.x = j * du;
			Vertices[i * n + j].TexC.y = i * dv;
		}
	}

	//
	// Create the indices.
	//

	Indices.resize(faceCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			Indices[k] = i * n + j;
			Indices[k + 1] = i * n + j + 1;
			Indices[k + 2] = (i + 1) * n + j;

			Indices[k + 3] = (i + 1) * n + j;
			Indices[k + 4] = i * n + j + 1;
			Indices[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}
	geometry->SetVertices(Vertices);
	geometry->SetIndices(Indices);

}

void GeometryHelper::CreateWave(shared_ptr<Geometry<Vertex::VertexFull>> geometry, UINT m, UINT n)
{
	CreateGrid(geometry, 1.f, 1.f, m, n, 1, 1);
}

void GeometryHelper::CreateCylinder(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{
	std::vector<VertexFull> Vertices;
	std::vector<UINT> Indices;

	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	UINT ringCount = stackCount + 1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for (UINT i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;

		// vertices of ring
		float dTheta = 2.0f * XM_PI / sliceCount;
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			VertexFull vertex;

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			vertex.Position = XMFLOAT3(r * c, y, r * s);

			vertex.TexC.x = (float)j / sliceCount;
			vertex.TexC.y = 1.0f - (float)i / stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			vertex.TangentU = XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius - topRadius;
			XMFLOAT3 bitangent(dr * c, -height, dr * s);

			XMVECTOR T = XMLoadFloat3(&vertex.TangentU);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&vertex.Normal, N);

			Vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	UINT ringVertexCount = sliceCount + 1;

	// Compute indices for each stack.
	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			Indices.push_back(i * ringVertexCount + j);
			Indices.push_back((i + 1) * ringVertexCount + j);
			Indices.push_back((i + 1) * ringVertexCount + j + 1);

			Indices.push_back(i * ringVertexCount + j);
			Indices.push_back((i + 1) * ringVertexCount + j + 1);
			Indices.push_back(i * ringVertexCount + j + 1);
		}
	}

	geometry->SetVertices(Vertices);
	geometry->SetIndices(Indices);
}

void GeometryHelper::CreateFullCylinder(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{
	CreateCylinder( geometry,  bottomRadius,  topRadius,  height,  sliceCount,  stackCount);
	BuildCylinderTopCap( geometry,  bottomRadius,  topRadius,  height,  sliceCount,  stackCount);
	BuildCylinderBottomCap( geometry,  bottomRadius,  topRadius,  height,  sliceCount,  stackCount);

}

void GeometryHelper::BuildCylinderTopCap(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{
	auto& vertices = geometry->GetVertices();
	auto& indices = geometry->GetIndices();

	uint32 baseIndex = vertices.size();

	float y = 0.5f * height;
	float dTheta = 2.0f * XM_PI / sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius * cosf(i * dTheta);
		float z = topRadius * sinf(i * dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		vertices.push_back(VertexFull(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// Cap center vertex.
	vertices.push_back(VertexFull(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// Index of center vertex.
	uint32 centerIndex = (uint32)vertices.size() - 1;

	for (uint32 i = 0; i < sliceCount; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i + 1);
		indices.push_back(baseIndex + i);
	}


}

void GeometryHelper::BuildCylinderBottomCap(shared_ptr<Geometry<Vertex::VertexFull>> geometry, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{

	auto& vertices = geometry->GetVertices();
	auto& indices = geometry->GetIndices();


	uint32 baseIndex = (uint32)vertices.size();
	float y = -0.5f * height;

	// vertices of ring
	float dTheta = 2.0f * XM_PI / sliceCount;
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius * cosf(i * dTheta);
		float z = bottomRadius * sinf(i * dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		vertices.push_back(VertexFull(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	// Cap center vertex.
	vertices.push_back(VertexFull(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

	// Cache the index of center vertex.
	uint32 centerIndex = (uint32)vertices.size() - 1;

	for (uint32 i = 0; i < sliceCount; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
}
