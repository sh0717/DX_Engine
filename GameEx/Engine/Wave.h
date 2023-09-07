#pragma once
class Wave: public Component
{
public:
	Wave(uint32 m, uint32 n, float dx, float dt, float speed, float damping);
	virtual ~Wave();

	virtual void  Update();
	void Disturb(UINT i, UINT j, float magnitude);

	uint32 GetRowCount() { return mNumRows; }
	uint32 GetColumnCount() { return mNumCols; }

	uint32 GetVertexCount() { return mVertexCount; }
	uint32 GetTriangleCount() { return mTriangleCount; }

	


private:

	uint32 mNumRows=0;
	uint32 mNumCols=0;
	uint32 mVertexCount=0;
	uint32 mTriangleCount = 0;

	float mSpeed;

	float _k1 = 0.f;
	float _k2 = 0.f;
	float _k3 = 0.f;
	float _timeStep = 0.f;
	float _spatialStep = 0.f;

	vector<XMFLOAT3> _prevSolution;
	vector<XMFLOAT3> _currSolution;
	vector<XMFLOAT3> _normals;
	vector<XMFLOAT3> _tangentX;

};

