#pragma once
#include <bitset>
#include <optional>

struct RawDelta
{
	int x, y;
	RawDelta() = default;
};

struct InputHandler
{
private:
	//MOUSE
	bool leftIsPressed = false;
	std::pair<int, int> lastLeftMouseClick;

	RawDelta rd = {};

	//KEYBOARD
	static constexpr unsigned int keys = 256;
	std::bitset<keys> keystates;
public:
	bool KeyIsPressed(unsigned char keycode) const;
	void OnkeyPressed(unsigned int keycode);
	void OnkeyReleased(unsigned char keycode);

	void OnRawDelta(int dx, int dy);
	void OnLeftPressed(int x, int y);
	void OnLeftRelease();
	void ClearRawData();

	bool LeftIsClicked() const;
	std::pair<int, int> GetMousePos() const;
	std::optional<RawDelta> ReadRawDelta();
};

inline std::optional<RawDelta> InputHandler::ReadRawDelta()
{
	return this->rd;
}

inline bool InputHandler::KeyIsPressed(unsigned char keycode) const
{
	return keystates[keycode];
}

inline void InputHandler::OnkeyPressed(unsigned int keycode)
{
	keystates.set(keycode, true);
}

inline void InputHandler::OnkeyReleased(unsigned char keycode)
{
	keystates.set(keycode, false);
}

inline void InputHandler::OnRawDelta(int dx, int dy)
{
	this->rd.x = dx;
	this->rd.y = dy;
}

inline void InputHandler::OnLeftPressed(int x, int y)
{
	this->leftIsPressed = true;
	this->lastLeftMouseClick.first = x;
	this->lastLeftMouseClick.second = y;
}

inline void InputHandler::OnLeftRelease()
{
	this->leftIsPressed = false;
}

inline void InputHandler::ClearRawData()
{
	this->rd.x = 0;
	this->rd.y = 0;
}

inline std::pair<int, int> InputHandler::GetMousePos() const
{
	return this->lastLeftMouseClick;
}

inline bool InputHandler::LeftIsClicked() const
{
	return leftIsPressed;
}