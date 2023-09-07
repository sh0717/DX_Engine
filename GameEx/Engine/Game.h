#pragma once
//이 게임 데스크 를 채워서 런에 넣어야 한다 .
struct GameDesc
{
	shared_ptr<class IExecute> app = nullptr;
	wstring appName = L"MapleStroy";
	HINSTANCE hInstance = 0;
	HWND hWnd = 0;
	float width = 2000;
	float height = 1000;
	bool vsync = false;
	bool windowed = true;
	Color clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
};

class Game
{
	DECLARE_SINGLE(Game);
public:
	WPARAM Run(GameDesc& desc);

	GameDesc& GetGameDesc() { return _desc; }


	uint32 cullingnum = 0;
private:
	ATOM MyRegisterClass();
	BOOL InitInstance(int cmdShow);

	void Update();
	void ShowFps();

	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	
private:
	GameDesc _desc;
};

