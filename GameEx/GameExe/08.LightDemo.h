#pragma once


class LightDemo :public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	
private:

	shared_ptr<Shader> lightShader;
	

	

	shared_ptr<DefaultObject> _obj;
	shared_ptr<DefaultObject> _obj2;
	shared_ptr<DefaultObject> mCamera;



	shared_ptr<DefaultObject> cubeObj;
};

