#include "Time.h"
using namespace std::chrono;

Time::Time()
{
	time = steady_clock::now();
}

float Time::Mark()
{
	const auto old = time;
	time = steady_clock::now();
	const duration<float> frameTime = time - old;
	return frameTime.count();
}

float Time::Peek()
{
	return duration<float>(steady_clock::now() - time).count();
}
