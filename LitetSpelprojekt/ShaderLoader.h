#pragma once
#include <d3d11.h>
#include <fstream>
#include <string>

bool LoadVertexShader(ID3D11Device& device, ComPtr<ID3D11VertexShader>& vertexShader, std::string path, std::string& vertexShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(path, std::ios::binary);

	if (!reader.is_open())
		return false;

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device.CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vertexShader)))
		return false;

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
		return false;

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device.CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pixelShader)))
		return false;

	shaderData.clear();
	reader.close();
	return true;
};