#pragma once
//#include "IExecute.h"
using namespace std;
class TriangleDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;


	shared_ptr<Shader> _shader;
	vector<Vertex::VertexData> _vertices;
	shared_ptr<VertexBuffer> _buffer;
};




