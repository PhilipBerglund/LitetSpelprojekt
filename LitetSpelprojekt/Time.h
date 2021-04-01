#pragma once
#include <chrono>

class Time
{

private:
	std::chrono::steady_clock::time_point time;

public:
	Time();
	float Mark();
	float Peek();

};