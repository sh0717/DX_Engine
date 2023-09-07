#include "pch.h"
#include "TriangleDemo.h"

void TriangleDemo::Init()
{
	//return;
	_shader = make_shared<Shader>(L"ttt.fx");

	{
		_vertices.resize(3);

		_vertices[0].position = Vec3{ -0.5f, 0.f, 0.f };
		_vertices[1].position = Vec3{ 0.f, 0.5f, 0.f };
		_vertices[2].position = Vec3{ 0.5f, 0.5f, 0.f };
	}

	_buffer = make_shared<VertexBuffer>(DEVICE);
	_buffer->CreateBuffer(_vertices);
}

void TriangleDemo::Update()
{

}

void TriangleDemo::Render()
{
	//return;
	uint32 stride = _buffer->GetStride();
	uint32 offset = _buffer->GetOffset();

	CONTEXT->IASetVertexBuffers(0, 1, _buffer->GetComPtr().GetAddressOf(), &stride, &offset);

	//_buffer->GetCount()
	//_shader->Draw(1, 0, 3);
	_shader->Draw(0, 1, 3);
	_shader->Draw(0,0, 3);
}
