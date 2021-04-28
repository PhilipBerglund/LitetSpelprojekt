#include "Window.h"
#include "Game.h"
#include "Timer.h"

#include "SoundHandler.h"
#include <utility>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);

	UINT WIDTH = GetSystemMetrics(SM_CXSCREEN);
	UINT HEIGHT = GetSystemMetrics(SM_CYSCREEN);

	bool windowed = false;

	if (windowed)
	{
		//WIDTH = 1680;
		//HEIGHT = 1050;
	}
	
	LPCWSTR windowTitle = L"Jessica Woolf: Murder Mysteries";
	Window window;

	WindowInitializer winInit;
	winInit.Initialize(window, WIDTH, HEIGHT, windowTitle, hInstance);
	SetCursorPos(WIDTH / 2, HEIGHT / 2);

	Graphics::Initialize(WIDTH, HEIGHT, window.GetWindowHandle(), windowed);

	Game* game = new Game(window.GetWindowHandle(), WIDTH, HEIGHT);

	Timer timer;
	float dt = 0;

	MSG msg = {};

	SoundHandler soundHandler;
	soundHandler.Initialize();
	//soundHandler.Start();

	while (msg.message != WM_QUIT)
	{	
		timer.Start();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);

		if (msg.wParam == VK_RETURN)
			break;

		game->Render(dt);
		dt = (float)timer.DeltaTime();
	}

	//soundHandler.Stop();
	delete game;
	return 0;
}