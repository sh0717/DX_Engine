#pragma once


class BookTextureDemo :public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;
	void CreateKachujin();
	

private:



	

	

	
	shared_ptr<DefaultObject> mCamera;
	shared_ptr<DefaultObject> mOverlapCamera;

	


	shared_ptr<DefaultObject> cubeObj;
	shared_ptr<Shader> modelAnimShader;
	shared_ptr<Shader> lightShader;
	shared_ptr<Shader> modelShader;
	shared_ptr<Shader> waterlightShader;
	
	shared_ptr<Shader> treeShader;


	shared_ptr<DefaultObject> _obj;


	bool show_demo_window = true;
	bool show_another_window = false;
	Vec4 clear_color = Vec4(0.f);
};

