#pragma once


class NormalTestDemo :public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	
private:

	shared_ptr<Shader> mShader;




	Matrix world = Matrix::Identity;

	shared_ptr<DefaultObject> mCamera;
	shared_ptr<Texture> mTexture=nullptr;
	shared_ptr<Mesh> mMesh = nullptr;
	Vec3 _lightDir = Vec3(0.f, 1.f, 0.f);
};

