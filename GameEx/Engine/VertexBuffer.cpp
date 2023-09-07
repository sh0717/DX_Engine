#include "pch.h"
#include "VertexBuffer.h"


VertexBuffer::VertexBuffer() 
	:mDevice(DEVICE)
{}

VertexBuffer::VertexBuffer(ComPtr<ID3D11Device> device)
	:mDevice(device)
{

}

VertexBuffer::~VertexBuffer()
{

}
