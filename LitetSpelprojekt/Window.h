#pragma once
#include <Windows.h>

class Window
{
private:
	UINT width;
	UINT height;
	HWND hWnd;
	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window(UINT width, UINT height, LPCWSTR title, HINSTANCE instance);
	~Window();
	HWND GetWindowHandle() const;
};