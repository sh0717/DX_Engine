#include "pch.h"
#include "InstancingBuffer.h"

InstancingBuffer::InstancingBuffer()
{
	CreateBuffer();
}

InstancingBuffer::~InstancingBuffer()
{

}

void InstancingBuffer::CreateBuffer(uint32 maxCount /*= MAX_MESH_INSTANCE*/)
{
	mMaxCount = maxCount;
	mIB = make_shared<VertexBuffer>();

	vector<InstancingData> temp(maxCount);
	mIB->CreateBuffer(temp, /*slot*/1, /*cpuWrite*/true);
}

void InstancingBuffer::ClearData()
{
	mData.clear();
}

void InstancingBuffer::AddData(InstancingData& data)
{
	mData.push_back(data);
}

void InstancingBuffer::PushData()
{
	const uint32 dataCount = GetCount();
	if (dataCount > mMaxCount)
		CreateBuffer(dataCount);

	D3D11_MAPPED_SUBRESOURCE subResource;

	CONTEXT->Map(mIB->GetComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		::memcpy(subResource.pData, mData.data(), sizeof(InstancingData) * dataCount);
	}
	CONTEXT->Unmap(mIB->GetComPtr().Get(), 0);

	mIB->PushData();
}
