#pragma once


class TexutreBufferDemo :public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;
	
	
	ComPtr<ID3D11ShaderResourceView> MakeComputeShaderTexture();
private:



	shared_ptr<Shader> Cshader;

	

	
	shared_ptr<DefaultObject> mCamera;
	//shared_ptr<DefaultObject> mCameraOverlap;
	shared_ptr<DefaultObject> UICamera;
	


	shared_ptr<DefaultObject> cubeObj;
	shared_ptr<Shader> modelAnimShader;
	shared_ptr<Shader> lightShader;
	

	shared_ptr<Shader> modelShader;
	shared_ptr<Shader> tessilShader;
	shared_ptr<Shader> terrainShader;
	

	shared_ptr<Shader> DebugShader;
	shared_ptr<Shader> DisplacementShaer;

	shared_ptr<DefaultObject> _obj;


	bool show_demo_window = true;
	bool show_another_window = false;
	Vec4 clear_color = Vec4(0.f);
};

