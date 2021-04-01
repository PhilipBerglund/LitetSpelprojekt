#pragma once
#include <Windows.h>

class Window
{
private:
	int width;
	int height;
	HWND hWnd;
	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window() = default;
	Window(UINT width, UINT height, LPCWSTR title, HINSTANCE instance);
	~Window();
	void SetTitle(LPCSTR title);
	HWND GetWindowHandle() const;
};