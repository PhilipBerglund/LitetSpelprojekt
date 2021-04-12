#pragma once
#include <Windows.h>
#include <vector>

class Window
{
private:
	UINT width;
	UINT height;
	HWND hWnd;
	std::vector<char> rawBuffer;
	bool cursorEnabled = true;

	LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window(UINT width, UINT height, LPCWSTR title, HINSTANCE instance);
	~Window();
	std::pair<int, int> GetRawInput(LPARAM lParam);
	std::pair<int, int> GetMousePos(LPARAM lParam);
	HWND GetWindowHandle() const;
	void EnableCursor();
	void DisableCursor();
};