#pragma once
#include <vector>
#include <utility>

using X = void(*)(void*);

enum class EventType
{
	RESET,
	RESETCAMERA,
	STATECHANGE,
	LEFTCLICK,
	LEFTUP,
	MOUSEMOVE,
	BUTTONDOWN,
	BUTTONUP,
	W_DOWN,
	A_DOWN,
	S_DOWN,
	D_DOWN,
	E_DOWN,
	W_UP,
	A_UP,
	S_UP,
	D_UP,
	ESC
};

class Event
{
	template<class C, void(C::* M)() = C::OnEvent>
	static void Invoke(void* instance)
	{
		(static_cast<C*>(instance)->*M)();
	}

public:
	template<class C, void(C::* M)() = &C::OnEvent>
	static void Bind(C* instance, EventType event)
	{
		auto pair = std::make_pair(&Invoke<C, M>, instance);

		switch (event)
		{
		case EventType::ESC:
			escape.push_back(pair);
			break;

		case EventType::RESET:
			reset.push_back(pair);
			break;

		case EventType::RESETCAMERA:
			resetCamera.push_back(pair);
			break;

		case EventType::STATECHANGE:
			stateChange.push_back(pair);
			break;

		case EventType::BUTTONDOWN:
			buttonDown.push_back(pair);
			break;

		case EventType::LEFTCLICK:
			leftClick.push_back(pair);
			break;

		case EventType::LEFTUP:
			leftUp.push_back(pair);
			break;

		case EventType::MOUSEMOVE:
			mouseMove.push_back(pair);
			break;

		case EventType::W_DOWN:
			W.push_back(pair);
			break;

		case EventType::A_DOWN:
			A.push_back(pair);
			break;

		case EventType::S_DOWN:
			S.push_back(pair);
			break;

		case EventType::D_DOWN:
			D.push_back(pair);
			break;

		case EventType::E_DOWN:
			E.push_back(pair);
			break;
		}
	}

	static void DispatchEvent(EventType event)
	{
		currentEvent = event;

		switch (event)
		{
		case EventType::RESET:
			for (auto pair : reset)
				(pair.first)(pair.second);
			break;

		case EventType::RESETCAMERA:
			for (auto pair : resetCamera)
				(pair.first)(pair.second);
			break;

		case EventType::STATECHANGE:
			for (auto pair : stateChange)
				(pair.first)(pair.second);
			break;

		case EventType::BUTTONDOWN:
			for (auto pair : buttonDown)
				(pair.first)(pair.second);
			break;

		case EventType::BUTTONUP:
			for (auto pair : buttonDown)
				(pair.first)(pair.second);
			break;

		case EventType::LEFTCLICK:
			for (auto pair : leftClick)
				(pair.first)(pair.second);
			break;

		case EventType::LEFTUP:
			for (auto pair : leftUp)
				(pair.first)(pair.second);
			break;

		case EventType::MOUSEMOVE:
			for (auto pair : mouseMove)
				(pair.first)(pair.second);
			break;

		case EventType::W_DOWN:
			for (auto pair : W)
				(pair.first)(pair.second);
			break;

		case EventType::A_DOWN:
			for (auto pair : A)
				(pair.first)(pair.second);
			break;

		case EventType::S_DOWN:
			for (auto pair : S)
				(pair.first)(pair.second);
			break;

		case EventType::D_DOWN:
			for (auto pair : D)
				(pair.first)(pair.second);
			break;

		case EventType::E_DOWN:
			for (auto pair : E)
				(pair.first)(pair.second);

			break;
		case EventType::W_UP:
			for (auto pair : W)
				(pair.first)(pair.second);
			break;

		case EventType::A_UP:
			for (auto pair : A)
				(pair.first)(pair.second);
			break;

		case EventType::S_UP:
			for (auto pair : S)
				(pair.first)(pair.second);
			break;

		case EventType::D_UP:
			for (auto pair : D)
				(pair.first)(pair.second);
			break;

		case EventType::ESC:
			for (auto pair : escape)
				(pair.first)(pair.second);
			break;
		}
	}

	static EventType GetCurrentEvent()
	{
		return currentEvent;
	}

private:
	static EventType currentEvent;
	static std::vector<std::pair<X, void*>> escape;
	static std::vector<std::pair<X, void*>> reset;
	static std::vector<std::pair<X, void*>> resetCamera;
	static std::vector<std::pair<X, void*>> stateChange;
	static std::vector<std::pair<X, void*>> buttonDown;
	static std::vector<std::pair<X, void*>> leftClick;
	static std::vector<std::pair<X, void*>> leftUp;
	static std::vector<std::pair<X, void*>> mouseMove;
	static std::vector<std::pair<X, void*>> W;
	static std::vector<std::pair<X, void*>> A;
	static std::vector<std::pair<X, void*>> S;
	static std::vector<std::pair<X, void*>> D;
	static std::vector<std::pair<X, void*>> E;
};