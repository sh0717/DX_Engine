#pragma once


//메쉬에는 vb나 ib 같은 거를 넣어놀 수 있다. 
//모양 

class Mesh:public ResourceBase
{
public:
	Mesh();
	virtual ~Mesh();

	void CreateQuad();
	void CreateCube();
	void CreateGrid(int32 sizeX, int32 sizeY);
	void CreateSphere();

	shared_ptr<VertexBuffer> GetVB() { return mVB; }
	shared_ptr<IndexBuffer> GetIB() { return mIB; }

	void SetVB(shared_ptr<VertexBuffer> vb) {mVB = vb;}
	void SetIB(shared_ptr<IndexBuffer> ib) { mIB = ib; }
	
	//if you set geometry the vertex buffer and index buffer are made automatically
	void SetGeometry(shared_ptr<Geometry<VertexFull>> geo,bool CPUWrite= false , bool GpuWrite =false) {
		mGeo = geo; 
		CreateBuffers(CPUWrite,GpuWrite); 
	}
	
	

	
private:
	void CreateBuffers(bool CPUWrite = false, bool GpuWrite = false);

private:

	shared_ptr<Geometry<VertexFull>>  mGeo;
	shared_ptr<VertexBuffer> mVB;
	shared_ptr<IndexBuffer> mIB;

};

