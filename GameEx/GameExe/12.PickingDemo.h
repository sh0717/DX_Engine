#pragma once


class PickingDemo :public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;
	void CreateKachujin();
	

private:



	

	

	
	shared_ptr<DefaultObject> mCamera;


	vector<shared_ptr<DefaultObject>> mObjs;


	shared_ptr<DefaultObject> cubeObj;
	shared_ptr<Shader> modelShader;
	shared_ptr<Shader> lightShader;

	shared_ptr<Shader> tankShader;

	shared_ptr<DefaultObject> _obj;


	bool show_demo_window = true;
	bool show_another_window = false;
	Vec4 clear_color = Vec4(0.f);
};

