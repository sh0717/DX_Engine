#include "pch.h"
#include "SceneManager.h"


void SceneManager::Init()
{

}

void SceneManager::Update()
{
	if (_currentScene == nullptr)
		return;

	_currentScene->Update();
	_currentScene->LateUpdate();

	_currentScene->RenderShadowMap();
	_currentScene->Render();
}
