#include "Window.h"
#include "Game.h"
#include "Timer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;
	LPCWSTR windowTitle = L"LILLA SPEL";
	Window window = Window(WIDTH, HEIGHT, windowTitle, hInstance);

	Game game = Game(window.GetWindowHandle(), WIDTH, HEIGHT);
	Timer timer;
	float dt = 0;

	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{	
		timer.Start();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_KEYDOWN)
			game.CatchInput(msg.wParam, true);

		if (msg.message == WM_KEYUP)
			game.CatchInput(msg.wParam, false);

		if (msg.wParam == VK_RETURN)
		{
			return 0;
		}

		game.Update(dt);
		game.Render();
		dt = (float)timer.DeltaTime();
	}

	return 0;
}