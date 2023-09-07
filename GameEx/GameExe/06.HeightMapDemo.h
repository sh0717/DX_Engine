#pragma once


class HeightMapDemo :public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	
private:

	shared_ptr<Shader> mShader;


	shared_ptr<Geometry<Vertex::VertexTextureData>> mGeo;
	shared_ptr<VertexBuffer> mVB;
	shared_ptr<IndexBuffer> mIB;

	Matrix world = Matrix::Identity;

	shared_ptr<DefaultObject> mCamera;


	shared_ptr<Texture> grassTexture=nullptr;
	shared_ptr<Texture> HeightMap = nullptr;

};

