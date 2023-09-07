#pragma once

class VertexBuffer;

#define MAX_MESH_INSTANCE 500
//�ϴ��� instancing �ҰŴ� world 
struct InstancingData
{
	Matrix world;
};

//instancingbuffer �� vertexbuffer �� �����ε� �̰Ÿ� slot 1�� �����̴ϴ�.
class InstancingBuffer
{

public:
	InstancingBuffer();
	~InstancingBuffer();

private:
	void CreateBuffer(uint32 maxCount = MAX_MESH_INSTANCE);

public:
	void ClearData();
	void AddData(InstancingData& data);

	void PushData();

public:
	uint32		GetCount() { return static_cast<uint32>(mData.size()); }
	shared_ptr<VertexBuffer>	GetInstancingBuffer() { return mIB; }

	void	SetID(uint64 instanceId) { mID = instanceId; }
	uint64	GetID() { return mID; }

private:
	uint64						mID = 0;
	shared_ptr<VertexBuffer>	mIB;
	uint32						mMaxCount = 0;
	vector<InstancingData>		mData;



};

