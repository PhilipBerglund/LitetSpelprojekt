#pragma once
#include <d3d11.h>
#include <fstream>
#include "Print.h"

bool LoadVertexShader(ID3D11Device& device, ComPtr<ID3D11VertexShader>& vertexShader, std::string path, std::string& vertexShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary);

	if (!reader.is_open())
	{
		Error("- COULD NOT OPEN FILE: " + path + " -");
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = device.CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vertexShader);
	if FAILED(hr)
	{
		Error("- FAILED TO CREATE VERTEX SHADER -");
		return false;
	}

	vertexShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	return true;
};

bool LoadPixelShader(ID3D11Device& device, ComPtr<ID3D11PixelShader>& pixelShader, std::string path)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary);

	if (!reader.is_open())
	{
		Error("- COULD NOT OPEN FILE: " + path + " -");
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	HRESULT hr = device.CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pixelShader);
	if FAILED(hr)
	{
		Error("- FAILED TO CREATE PIXEL SHADER -");
		return false;
	}

	shaderData.clear();
	reader.close();
	return true;
};