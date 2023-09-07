#pragma once


class ModelInitDemo :public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	void CreateTank();
	void CreateTurret();

private:

	shared_ptr<Shader> lightShader;


	

	shared_ptr<DefaultObject> _obj;
	shared_ptr<DefaultObject> _obj2;
	shared_ptr<DefaultObject> mCamera;



	shared_ptr<DefaultObject> cubeObj;



	shared_ptr<Shader> modelShader;

	shared_ptr<DefaultObject> modelObj;
	shared_ptr<DefaultObject> turretObj;



	bool show_demo_window = true;
	bool show_another_window = false;
	Vec4 clear_color = Vec4(0.f);
};

