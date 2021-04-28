#include "Window.h"
#include "GameSettings.h"

UINT Window::width = 0;
UINT Window::height = 0;
HWND Window::hWnd = {};
std::vector<char> Window::rawBuffer = {};
bool Window::cursorEnabled = true;
std::pair<int, int> Window::rawInput;

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (msg == WM_NCCREATE)
	{
		LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		wnd = reinterpret_cast<Window*>(cs->lpCreateParams);

		SetLastError(0);

		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}

	else
		return wnd->MessageHandler(hWnd, msg, wParam, lParam);

	if (wnd)
		return wnd->MessageHandler(hWnd, msg, wParam, lParam);

	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	
	case WM_INPUT:
		StashRawInput(lParam);
		Event::DispatchEvent(EventType::MOUSEMOVE);
		break;

	case WM_KEYDOWN:
		Event::DispatchEvent(EventType::BUTTONDOWN);

		switch (wParam)
		{
		case('W'):
			Event::DispatchEvent(EventType::W_DOWN);
			break;

		case('A'):
			Event::DispatchEvent(EventType::A_DOWN);
			break;

		case('S'):
			Event::DispatchEvent(EventType::S_DOWN);
			break;

		case('D'):
			Event::DispatchEvent(EventType::D_DOWN);
			break;

		case('E'):
			Event::DispatchEvent(EventType::E_DOWN);
			break;
		}

		break;
	
	case WM_KEYUP:
		Event::DispatchEvent(EventType::BUTTONUP);
		
		switch (wParam)
		{
		case('W'):
			Event::DispatchEvent(EventType::W_UP);
			break;

		case('A'):
			Event::DispatchEvent(EventType::A_UP);
			break;

		case('S'):
			Event::DispatchEvent(EventType::S_UP);
			break;

		case('D'):
			Event::DispatchEvent(EventType::D_UP);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		Event::DispatchEvent(EventType::LEFTCLICK);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::StashRawInput(LPARAM lParam)
{
	std::pair<int, int> coords;
	UINT size = 0;

	if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
		return;

	rawBuffer.resize(size);

	if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
		return;

	auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
	if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		coords = { ri.data.mouse.lLastX, ri.data.mouse.lLastY };

	rawInput = coords;
}

std::optional<std::pair<int, int>> Window::GetRawInput()
{
	return rawInput;
}

std::pair<int, int> Window::GetMousePos()
{
	std::pair<int, int> points;
	LPPOINT point = new POINT;

	if (GetCursorPos(point) == TRUE)
	{
		points.first = point->x;
		points.second = point->y;
	}

	if (ScreenToClient(hWnd, point))
	{
		points.first = point->x;
		points.second = point->y;
	}

	return points;
}
//
//std::pair<int, int> Window::GetMousePos(LPARAM lParam)
//{
//	POINTS pt = MAKEPOINTS(lParam);
//	return std::pair<int, int>(pt.x, pt.y);
//}

HWND Window::GetWindowHandle()
{
	return hWnd;
}

void Window::EnableCursor()
{
	if (!cursorEnabled)
	{
		ClipCursor(nullptr);
		SetCursorPos(width / 2, height / 2);
		cursorEnabled = true;
	}
}

void Window::DisableCursor()
{
	if (cursorEnabled)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
		cursorEnabled = false;
	}
}

float Window::GetWidth()
{
	return (float)width;
}

float Window::GetHeight()
{
	return (float)height;
}

void WindowInitializer::Initialize(Window& window, UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
{
	window.width = width;
	window.height = height;

	const wchar_t* className = L"Window Class";

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = window.WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = className;

	RegisterClass(&wc);

	window.hWnd = CreateWindowEx(0, className, title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, instance, this);

	//hWnd = CreateWindow(className, title, 0, 0, 0, width, height, nullptr, nullptr, instance, this);

	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));
	ShowWindow(window.hWnd, SW_SHOWDEFAULT);
	ShowCursor(false);

	Event::Bind(this, EventType::STATECHANGE);
}

void WindowInitializer::OnEvent()
{
	switch (GameSettings::GetState())
	{
	case GameState::INGAME:
		Window::DisableCursor();
		break;

	case GameState::MAINMENU:
		Window::EnableCursor();
		break;
	}
}
