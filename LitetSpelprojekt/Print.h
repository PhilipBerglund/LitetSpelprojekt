#pragma once
#include <iostream>
#include <string>

inline void Error(std::string string)
{
	std::cerr << string << std::endl;
}

inline void Print(std::string string)
{
	std::cout << string << std::endl;
}