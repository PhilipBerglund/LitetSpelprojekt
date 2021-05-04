#pragma once
#include <Windows.h>
#include <vector>
#include "Event.h"
#include <optional>

class Window
{
	friend struct WindowInitializer;
private:
	static UINT width;
	static UINT height;
	static HWND hWnd;
	static std::vector<char> rawBuffer;
	static std::pair<int, int> rawInput;

	static void StashRawInput(LPARAM lParam);
	static LRESULT MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	~Window();
	static std::optional<std::pair<int, int>> GetRawInput();
	static std::pair<int, int> GetMousePos();
	static HWND GetWindowHandle();
	static void EnableCursor();
	static void DisableCursor();
	static bool cursorEnabled;
	static float GetWidth();
	static float GetHeight();
	static std::pair<int, int> GetCenter();
};

struct WindowInitializer
{
	void Initialize(Window& window, UINT width, UINT height, LPCWSTR title, HINSTANCE instance);
	void OnEvent();
};