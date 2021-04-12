#include "Window.h"
#include "Game.h"
#include "Timer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;
	LPCWSTR windowTitle = L"LILLA SPEL";
	Window window = Window(WIDTH, HEIGHT, windowTitle, hInstance);

	Game game = Game(window.GetWindowHandle(), WIDTH, HEIGHT);
	Timer timer;
	float dt = 0;

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{	
		timer.Start();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);

		if (msg.wParam == VK_RETURN)
			return 0;

		if (msg.message == WM_KEYDOWN)
			game.CatchInput((unsigned char)msg.wParam);

		if (msg.message == WM_KEYUP)
			game.CatchInput((unsigned char)msg.wParam, false);

		if (msg.message == WM_LBUTTONDOWN)
			game.CatchInput(window.GetMousePos(msg.lParam));

		if (msg.message == WM_LBUTTONUP)
			game.CatchInput(window.GetMousePos(msg.lParam), false);

		if (msg.message == WM_INPUT)
			game.CatchRawInput(window.GetRawInput(msg.lParam));

		if (game.GetState() == GameState::MAINMENU)
			window.EnableCursor();
		
		SetCursorPos(WIDTH / 2, HEIGHT / 2);
		game.Render(dt);
		dt = (float)timer.DeltaTime();
	}

	return 0;
}