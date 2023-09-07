#pragma once

#include "Scene.h"
class SceneManager

{
	DECLARE_SINGLE(SceneManager);

public:
	
	void Init();

	void Update();

	/// <summary>
	/// SCene의 자식들을 사용할 수도 있따 나중에 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="scene"></param>
	template<typename T>
	void ChangeScene(shared_ptr<T> scene)
	{
		_currentScene = scene;
		scene->Start();
	}

	shared_ptr<Scene> GetCurrentScene() { return _currentScene; }

private:
	shared_ptr<Scene> _currentScene = make_shared<Scene>();

};

