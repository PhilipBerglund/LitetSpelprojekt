#pragma once
#include <bitset>

struct InputHandler
{
public:
	bool KeyIsPressed(unsigned char keycode) const;
	void OnkeyPressed(unsigned int keycode);
	void OnkeyReleased(unsigned char keycode);
private:
	static constexpr unsigned int keys = 256;
	std::bitset<keys> keystates;
};

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