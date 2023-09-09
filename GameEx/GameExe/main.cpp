#include "pch.h"
#include "main.h"
//#include "Engine/Game.h"
#include "TriangleDemo.h"
#include "02.CameraDemo.h"
#include "03.TextureDemo.h"
#include "04.SamplerDemo.h"

#include "05.ColorBoxDemo.h"
#include "06.HeightMapDemo.h"
#include "07.NormalTestDemo.h"
#include "08.LightDemo.h"
#include "09.ModelInitDemo.h"
#include "10.AnimDemo.h"
#include "11.InstancingDemo.h"
#include "12.PickingDemo.h"
#include "b1.TextureLightDemo.h"
#include "b2.mirrorDemo.h"
#include "b3.TessilDemo.h"
#include "b4.TessilDemo.h"
#include "b5.ShadowDemo.h"
#include "b6.TextureBufferDemo.h"
GameDesc b1TextrueDemodesc;
GameDesc b2mirrordesc;
GameDesc b3tessildesc;
GameDesc b4tessildesc;
GameDesc b5Shadowdesc;
GameDesc b6Texturedesc;

class Descs {
public:

	

	
	

	static void Initi(HINSTANCE inst) {
		
		b1TextrueDemodesc.appName = L"GameCoding";
		b1TextrueDemodesc.hInstance = inst;
		b1TextrueDemodesc.vsync = false;
		b1TextrueDemodesc.hWnd = NULL;
		b1TextrueDemodesc.width = 1200;
		b1TextrueDemodesc.height = 800;
		b1TextrueDemodesc.clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
		b1TextrueDemodesc.app = make_shared<BookTextureDemo>();

		b2mirrordesc.appName = L"GameCoding";
		b2mirrordesc.hInstance = inst;
		b2mirrordesc.vsync = false;
		b2mirrordesc.hWnd = NULL;
		b2mirrordesc.width = 1200;
		b2mirrordesc.height = 800;
		b2mirrordesc.clearColor = Color(0.8f, 0.5f, 0.5f, 0.5f);
		b2mirrordesc.app = make_shared<MirrorDemo>();

		b3tessildesc.appName = L"GameCoding";
		b3tessildesc.hInstance = inst;
		b3tessildesc.vsync = false;
		b3tessildesc.hWnd = NULL;
		b3tessildesc.width = 1200;
		b3tessildesc.height = 800;
		b3tessildesc.clearColor = Color(0.8f, 0.5f, 0.5f, 0.5f);
		b3tessildesc.app = make_shared<TessilDemo>();

		b4tessildesc.appName = L"GameCoding";
		b4tessildesc.hInstance = inst;
		b4tessildesc.vsync = false;
		b4tessildesc.hWnd = NULL;
		b4tessildesc.width = 1200;
		b4tessildesc.height = 800;
		b4tessildesc.clearColor = Color(0.8f, 0.5f, 0.5f, 0.5f);
		b4tessildesc.app = make_shared<TessilDemo2>();


		b5Shadowdesc.appName = L"GameCoding";
		b5Shadowdesc.hInstance = inst;
		b5Shadowdesc.vsync = false;
		b5Shadowdesc.hWnd = NULL;
		b5Shadowdesc.width = 1200;
		b5Shadowdesc.height = 800;
		b5Shadowdesc.clearColor = Color(0.8f, 0.5f, 0.5f, 0.5f);
		b5Shadowdesc.app = make_shared<ShadowDemo>();

		b6Texturedesc.appName = L"GameCoding";
		b6Texturedesc.hInstance = inst;
		b6Texturedesc.vsync = false;
		b6Texturedesc.hWnd = NULL;
		b6Texturedesc.width = 1200;
		b6Texturedesc.height = 800;
		b6Texturedesc.clearColor = Color(0.8f, 0.5f, 0.5f, 0.5f);
		b6Texturedesc.app = make_shared<TexutreBufferDemo>();

	}

};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Descs::Initi(hInstance);


	GameDesc CamearDemodesc;
	CamearDemodesc.appName = L"GameCoding";
	CamearDemodesc.hInstance = hInstance;
	CamearDemodesc.vsync = false;
	CamearDemodesc.hWnd = NULL;
	CamearDemodesc.width = 800;
	CamearDemodesc.height = 600;
	CamearDemodesc.clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
	CamearDemodesc.app = make_shared<CameraDemo>();


	GameDesc TextureDemoDesc;
	TextureDemoDesc.appName = L"GameCoding";
	TextureDemoDesc.hInstance = hInstance;
	TextureDemoDesc.vsync = false;
	TextureDemoDesc.hWnd = NULL;
	TextureDemoDesc.width = 800;
	TextureDemoDesc.height = 600;
	TextureDemoDesc.clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
	TextureDemoDesc.app = make_shared<TextureDemo>();


	GameDesc SamplerDemoDesc;
	SamplerDemoDesc.appName = L"Sampler";
	SamplerDemoDesc.hInstance = hInstance;
	SamplerDemoDesc.vsync = false;
	SamplerDemoDesc.hWnd = NULL;
	SamplerDemoDesc.width = 800;
	SamplerDemoDesc.height = 600;
	SamplerDemoDesc.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	SamplerDemoDesc.app = make_shared<SamplerDemo>();

	
	GameDesc ColorBoxDemoDesc;
	ColorBoxDemoDesc.appName = L"Sampler";
	ColorBoxDemoDesc.hInstance = hInstance;
	ColorBoxDemoDesc.vsync = false;
	ColorBoxDemoDesc.hWnd = NULL;
	ColorBoxDemoDesc.width = 800;
	ColorBoxDemoDesc.height = 600;
	ColorBoxDemoDesc.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	ColorBoxDemoDesc.app = make_shared<ColorBoxDemo>();


	GameDesc HeightDesc;
	HeightDesc.appName = L"Sampler";
	HeightDesc.hInstance = hInstance;
	HeightDesc.vsync = false;
	HeightDesc.hWnd = NULL;
	HeightDesc.width = 800;
	HeightDesc.height = 600;
	HeightDesc.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	HeightDesc.app = make_shared<HeightMapDemo>();


	GameDesc NormalSampleDesc;
	NormalSampleDesc.appName = L"Sampler";
	NormalSampleDesc.hInstance = hInstance;
	NormalSampleDesc.vsync = false;
	NormalSampleDesc.hWnd = NULL;
	NormalSampleDesc.width = 800;
	NormalSampleDesc.height = 600;
	NormalSampleDesc.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	NormalSampleDesc.app = make_shared<NormalTestDemo>();

	GameDesc LightDesc;
	LightDesc.appName = L"Sampler";
	LightDesc.hInstance = hInstance;
	LightDesc.vsync = false;
	LightDesc.hWnd = NULL;
	LightDesc.width = 800;
	LightDesc.height = 600;
	LightDesc.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	LightDesc.app = make_shared<LightDemo>();


	GameDesc ModelDesc;
	ModelDesc.appName = L"sdsdsss";
	ModelDesc.hInstance = hInstance;
	ModelDesc.vsync = false;
	ModelDesc.hWnd = NULL;
	ModelDesc.width = 800;
	ModelDesc.height = 600;
	ModelDesc.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	ModelDesc.app = make_shared<ModelInitDemo>();



	GameDesc AnimDemoDesc;
	AnimDemoDesc.appName = L"sdsdsss";
	AnimDemoDesc.hInstance = hInstance;
	AnimDemoDesc.vsync = false;
	AnimDemoDesc.hWnd = NULL;
	AnimDemoDesc.width = 800;
	AnimDemoDesc.height = 600;
	AnimDemoDesc.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	AnimDemoDesc.app = make_shared<AnimDemo>();

	GameDesc InstancingDemoDesc;
	InstancingDemoDesc.appName = L"123";
	InstancingDemoDesc.hInstance = hInstance;
	InstancingDemoDesc.vsync = false;
	InstancingDemoDesc.hWnd = NULL;
	InstancingDemoDesc.width = 1200;
	InstancingDemoDesc.height = 800;
	InstancingDemoDesc.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	InstancingDemoDesc.app = make_shared<InstancingDemo>();



	GameDesc PickDemo;
	PickDemo.appName = L"123";
	PickDemo.hInstance = hInstance;
	PickDemo.vsync = false;
	PickDemo.hWnd = NULL;
	PickDemo.width = 1200;
	PickDemo.height = 800;
	PickDemo.clearColor = Color(0.7f, 0.5f, 0.5f, 0.5f);
	PickDemo.app = make_shared<PickingDemo>();

		//03.CameraDemo 角青
		//GAME->Run(CamearDemodesc);

		//04.TextureDemo 角青
		//GAME->Run(HeightDesc);


		//Samplerdemo 角青
		//GAME->Run(ColorBoxDemoDesc);


	   //07.NormalDemo 角青
		//GAME->Run(NormalSampleDesc);




	//GAME->Run(LightDesc);


	//GAME->Run(ModelDesc);

	//GAME->Run(AnimDemoDesc);

	//GAME->Run(InstancingDemoDesc);



	//GAME->Run(PickDemo);



	//GAME->Run(b1TextrueDemodesc);
	//GAME->Run(b2mirrordesc);
	//GAME->Run(b3tessildesc);
	//GAME->Run(b4tessildesc);
	//GAME->Run(b5Shadowdesc);
	GAME->Run(b6Texturedesc);
	return 0;
}