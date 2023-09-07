#include "pch.h"
#include "Wave.h"

Wave::Wave(uint32 m, uint32 n, float dx, float dt, float speed, float damping)
	:Component(eComponentType::Wave),mNumRows(m),mNumCols(n),mVertexCount(m*n),mTriangleCount((m-1)*(n-1)*2)
	,_timeStep(dt),_spatialStep(dx),mSpeed(speed)
{
	
	float d = damping * dt + 2.0f;
	float e = (speed * speed) * (dt * dt) / (dx * dx);
	_k1 = (damping * dt - 2.0f) / d;
	_k2 = (4.0f - 8.0f * e) / d;
	_k3 = (2.0f * e) / d;

	// In case Init() called again.


	_prevSolution.resize(m * n);
	_currSolution.resize(m * n);
	_normals.resize(m * n);
	_tangentX.resize(m * n);

	// Generate grid vertices in system memory.

	float halfWidth = (n - 1) * dx * 0.5f;
	float halfDepth = (m - 1) * dx * 0.5f;

	for (uint32 i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dx;

		for (uint32 j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			_prevSolution[i * n + j] = XMFLOAT3(x, 0.0f, z);
			_currSolution[i * n + j] = XMFLOAT3(x, 0.0f, z);
			_normals[i * n + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
			_tangentX[i * n + j] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		}
	}



}

Wave::~Wave()
{

}

void Wave::Update()
{
	static float t = 0;
	float dt = DT;
	
	static float tex_x_offset = 0.f;
	static float tex_y_offset = 0.f;

	tex_x_offset+=DT * 0.1;
	tex_y_offset+=DT * 0.1;

	// Accumulate time.
	t += dt;

	// Only update the simulation at the specified time step.
	if (t >= _timeStep)
	{	

		DWORD i = 5 + rand() % (mNumRows - 10);
		DWORD j = 5 + rand() % (mNumCols - 10);

		float r = (float)(rand()) / (float)RAND_MAX*0.1;

		Disturb(i, j, r);
		

		// Only update interior points; we use zero boundary conditions.
		for (DWORD i = 1; i < mNumRows - 1; ++i)
		{
			for (DWORD j = 1; j < mNumCols - 1; ++j)
			{
				// After this update we will be discarding the old previous
				// buffer, so overwrite that buffer with the new update.
				// Note how we can do this inplace (read/write to same element) 
				// because we won't need prev_ij again and the assignment happens last.

				// Note j indexes x and i indexes z: h(x_j, z_i, t_k)
				// Moreover, our +z axis goes "down"; this is just to 
				// keep consistent with our row indices going down.

				_prevSolution[i * mNumCols + j].y =
					_k1 * _prevSolution[i * mNumCols + j].y +
					_k2 * _currSolution[i * mNumCols + j].y +
					_k3 * (_currSolution[(i + 1) * mNumCols + j].y +
						_currSolution[(i - 1) * mNumCols + j].y +
						_currSolution[i * mNumCols + j + 1].y +
						_currSolution[i * mNumCols + j - 1].y);
			}
		}

		// We just overwrote the previous buffer with the new data, so
		// this data needs to become the current solution and the old
		// current solution becomes the new previous solution.
		_prevSolution.swap(_currSolution);

		t = 0.0f; // reset time

		//
		// Compute normals using finite difference scheme.
		//
		for (uint32 i = 1; i < mNumRows - 1; ++i)
		{
			for (uint32 j = 1; j < mNumCols - 1; ++j)
			{
				float l = _currSolution[i * mNumCols + j - 1].y;
				float r = _currSolution[i * mNumCols + j + 1].y;
				float t = _currSolution[(i - 1) * mNumCols + j].y;
				float b = _currSolution[(i + 1) * mNumCols + j].y;
				_normals[i * mNumCols + j].x = -r + l;
				_normals[i * mNumCols + j].y = 2.0f * _spatialStep;
				_normals[i * mNumCols + j].z = b - t;

				XMVECTOR n = ::XMVector3Normalize(::XMLoadFloat3(&_normals[i * mNumCols + j]));
				::XMStoreFloat3(&_normals[i * mNumCols + j], n);

				_tangentX[i * mNumCols + j] = XMFLOAT3(2.0f * _spatialStep, r - l, 0.0f);
				XMVECTOR T = ::XMVector3Normalize(::XMLoadFloat3(&_tangentX[i * mNumCols + j]));
				::XMStoreFloat3(&_tangentX[i * mNumCols + j], T);
			}
		}
	}


	shared_ptr<DefaultObject> obj = GetGameObject();
	shared_ptr<Mesh>  mesh = obj->GetMeshRenderer()->GetMesh();


	shared_ptr<VertexBuffer> buffer = mesh->GetVB();

	ComPtr<ID3D11Buffer> VB = buffer->GetComPtr();


	
	D3D11_MAPPED_SUBRESOURCE mappedData;
	CONTEXT->Map(VB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

	ID3D11Buffer* bbb = VB.Get();

	VertexFull* v = reinterpret_cast<VertexFull*>(mappedData.pData);
	for (uint32 i = 0; i < mVertexCount; ++i)
	{
	
		v[i].Position = _currSolution[i];
		v[i].Normal = _normals[i];

		// Derive tex-coords in [0,1] from position.
		v[i].TexC.x = 0.5f + _currSolution[i].x  / _spatialStep/60;
		v[i].TexC.y = 0.5f - _currSolution[i].z /_spatialStep/60;

		v[i].TexC.x += tex_x_offset;
		v[i].TexC.y +tex_y_offset;
	}

	CONTEXT->Unmap(VB.Get(), 0);

}

void Wave::Disturb(UINT i, UINT j , float magnitude)
{
	// Don't disturb boundaries.
	assert(i > 1 && i < mNumRows - 2);
	assert(j > 1 && j < mNumCols - 2);

	float halfMag = 0.5f * magnitude;

	// Disturb the ijth vertex height and its neighbors.
	_currSolution[i * mNumCols + j].y += magnitude;
	_currSolution[i * mNumCols + j + 1].y += halfMag;
	_currSolution[i * mNumCols + j - 1].y += halfMag;
	_currSolution[(i + 1) * mNumCols + j].y += halfMag;
	_currSolution[(i - 1) * mNumCols + j].y += halfMag;
}
