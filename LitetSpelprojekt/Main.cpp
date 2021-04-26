#include "Window.h"
#include "Game.h"
#include "Timer.h"
#include "SoundHandler.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);

	UINT WIDTH = GetSystemMetrics(SM_CXSCREEN);
	UINT HEIGHT = GetSystemMetrics(SM_CYSCREEN);

	//WIDTH = 1680;
	//HEIGHT = 1050;

	WIDTH = 1024;
	HEIGHT = 576;

	LPCWSTR windowTitle = L"LILLA SPEL";
	Window window = Window(WIDTH, HEIGHT, windowTitle, hInstance);
	SetCursorPos((float)WIDTH / 2, (float)HEIGHT / 2);

	Graphics::Initialize(WIDTH, HEIGHT, window.GetWindowHandle());

	Game* game = new Game(window.GetWindowHandle(), WIDTH, HEIGHT);

	Timer timer;
	float dt = 0;

	MSG msg = {};

	SoundHandler soundHandler;
	soundHandler.Initialize();
	soundHandler.Start();

	while (msg.message != WM_QUIT)
	{	
		timer.Start();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage(&msg);

		if (msg.wParam == VK_RETURN)
			break;
			
		if (msg.message == WM_KEYDOWN)
			game->CatchInput((unsigned char)msg.wParam);

		if (msg.message == WM_KEYUP)
			game->CatchInput((unsigned char)msg.wParam, false);

		if (msg.message == WM_LBUTTONDOWN)
			game->CatchInput(window.GetMousePos(msg.lParam));

		if (msg.message == WM_LBUTTONUP || msg.message == WM_MOUSEMOVE)
			game->CatchInput(window.GetMousePos(msg.lParam), false);

		if (msg.message == WM_INPUT && game->GetState() == GameState::INGAME)
			game->CatchRawInput(window.GetRawInput(msg.lParam));

		if (game->GetState() == GameState::MAINMENU && window.cursorEnabled == false ||
			game->GetState() == GameState::PAUSED && window.cursorEnabled == false)
			window.EnableCursor();

		if (game->GetState() == GameState::INGAME && window.cursorEnabled == true)
			window.DisableCursor();

		if (game->GetState() == GameState::INGAME)
			SetCursorPos(WIDTH / 2, HEIGHT / 2);

		game->Render(dt);
		dt = (float)timer.DeltaTime();
	}

	soundHandler.Stop();
	delete game;
	return 0;
}