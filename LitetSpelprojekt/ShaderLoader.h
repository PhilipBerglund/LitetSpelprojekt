#pragma once
#include <fstream>
#include "Print.h"
#include "Graphics.h"

inline bool LoadVertexShader(ComPtr<ID3D11VertexShader>& vertexShader, std::string path, std::string& vertexShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary | std::ios::beg);

	if (!reader.is_open())
	{
		Error("COULD NOT OPEN FILE: " + path);
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = Graphics::GetDevice().CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vertexShader);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE VERTEX SHADER");
		return false;
	}

	vertexShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	return true;
};

inline bool LoadPixelShader(ComPtr<ID3D11PixelShader>& pixelShader, std::string path)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary | std::ios::beg);

	if (!reader.is_open())
	{
		Error("COULD NOT OPEN FILE: " + path);
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = Graphics::GetDevice().CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pixelShader);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE PIXEL SHADER");
		return false;
	}

	shaderData.clear();
	reader.close();
	return true;
};