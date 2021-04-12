#include "Window.h"
#include <string>

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
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window(UINT width, UINT height, LPCWSTR title, HINSTANCE instance)
	:width(width), height(height)
{
	const wchar_t* className = L"Window Class";

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = className;

	RegisterClass(&wc);

	hWnd = CreateWindowEx(0, className, title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, instance, this);

	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	DisableCursor();
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

std::pair<int, int> Window::GetRawInput(LPARAM lParam)
{
	std::pair<int, int> coords;
	UINT size = 0;
	if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
		return coords;

	rawBuffer.resize(size);

	if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
		return coords;

	auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
	if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		return { ri.data.mouse.lLastX, ri.data.mouse.lLastY };

	return coords;
}

std::pair<int, int> Window::GetMousePos(LPARAM lParam)
{
	POINTS pt = MAKEPOINTS(lParam);
	return std::pair<int, int>(pt.x, pt.y);
}

HWND Window::GetWindowHandle() const
{
	return hWnd;
}

void Window::EnableCursor()
{
	if (!cursorEnabled)
	{
		while (::ShowCursor(TRUE) < 0);
		ClipCursor(nullptr);
		SetCursorPos(width / 2, height / 2);
		cursorEnabled = true;
	}
}

void Window::DisableCursor()
{
	if (cursorEnabled)
	{
		while (::ShowCursor(FALSE) >= 0);
		RECT rect;
		GetClientRect(hWnd, &rect);
		MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
		cursorEnabled = false;
	}
}