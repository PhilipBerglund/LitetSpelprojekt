#pragma once
#include <iostream>

template <typename T>
inline void Print(const T& value, std::string preText = "")
{
	std::cout << preText << value << std::endl;
}

inline void Print(bool value, std::string preText = "")
{
	std::cout << preText;

	if (value)
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
}

template <typename T>
inline void Print(const T vector[], const int& size, const std::string& preText = "")
{
	std::cout << preText << "{ " << vector[0];

	for (int i = 1; i < size; ++i)
		std::cout << ", " << vector[i];

	std::cout << " }" << std::endl;
}

inline void Print(std::string text)
{
	std::cout << text << std::endl;
}