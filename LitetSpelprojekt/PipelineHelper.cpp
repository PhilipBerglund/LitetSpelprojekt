#pragma once
#include "PipelineHelper.h"
#include <string>
#include <fstream>
#include <iostream>
#include <DirectXMath.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace DirectX;

bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("../Debug/VertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open vertex shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());
	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}

	vShaderByteCode = shaderData;

	shaderData.clear();
	reader.close();
	reader.open("../Debug/PixelShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open pixel shader file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());
	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
	{
		std::cerr << "Failed to create pixel shader!" << std::endl;
		return false;
	}

	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, 3, vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);

	return !FAILED(hr);
}


bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState* & sampler)
{
	D3D11_SAMPLER_DESC desc;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 16;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&desc, &sampler);
	return !FAILED(hr);
}


bool SetupPipeline(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout,
	ID3D11SamplerState*& sampler)
{
	std::string vShaderByteCode;
	if (!LoadShaders(device, vShader, pShader, vShaderByteCode))
	{
		std::cerr << "Error loading shaders!" << std::endl;
		return false;
	}

	if (!CreateInputLayout(device, inputLayout, vShaderByteCode))
	{
		std::cerr << "Error creating input layout!" << std::endl;
		return false;
	}

	if (!CreateSamplerState(device, sampler))
	{
		std::cerr << "Error creating sampler state!" << std::endl;
		return false;
	}

	return true;
}


bool SetupRasterizerState(ID3D11Device* device, ID3D11RasterizerState* &rastState)
{

	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0;
	rastDesc.SlopeScaledDepthBias = 0;
	rastDesc.DepthClipEnable = true;
	rastDesc.ScissorEnable = false;
	rastDesc.MultisampleEnable = false;
	rastDesc.AntialiasedLineEnable = false;

	HRESULT hr = device->CreateRasterizerState(&rastDesc, &rastState);
	return !FAILED(hr);
}


//bool CreateVertexBuffer(ID3D11Device* device, ID3D11Buffer*& vertexBuffer)
//{
//
//
//	D3D11_BUFFER_DESC bufferDesc;
//	bufferDesc.ByteWidth = sizeof(triangle);	//Datan blir lika stor som triangeln (sizeof)	/Arrayen
//	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;	//Går inte att ändras efter den skapats (som const)
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//Ska tolkas som vertex-buffer
//	bufferDesc.CPUAccessFlags = 0;						//CPU behöver inte läsa eller skriva
//	bufferDesc.MiscFlags = 0;
//	bufferDesc.StructureByteStride = 0;					//Inte relevant i vårt exempel	(sctructureBuffer)
//
//	D3D11_SUBRESOURCE_DATA data;
//	data.pSysMem = triangle;	//Pekare till triangeln
//	data.SysMemPitch = 0;		//Relevanta för texturer
//	data.SysMemSlicePitch = 0;	//   ---||---		(3D)
//
//	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);	//Måste ge data då buffern är immutable
//	return !FAILED(hr);
//}


bool CreateTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV)
{

	int tWidth, tHeight, channels;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* image = stbi_load("../Images/railroad.png", &tWidth, &tHeight, &channels, 4);

	std::vector<unsigned char> tData;
	tData.resize(tWidth * tHeight * channels);	//Skapar en resurs där vi sparar information från texturen, lika stor som höjd * bredd * kanaler (rgba)
	for (int h = 0; h < tHeight; h++)
	{
		for (int w = 0; w < tWidth; w++)
		{
			//en char har garanterat 8 bitar (1 byte)


			unsigned int pos0 = w * 4 + tWidth * 4 * h;

			tData[pos0 + 0] = image[pos0 + 0];
			tData[pos0 + 1] = image[pos0 + 1];
			tData[pos0 + 2] = image[pos0 + 2];
			tData[pos0 + 3] = image[pos0 + 3];
		}
	}


	D3D11_TEXTURE2D_DESC desc;
	desc.Width = tWidth;	//Storlek, höjd och bredd
	desc.Height = tHeight;
	desc.MipLevels = 1;		//Bara en verison av upplösning
	desc.ArraySize = 1;		//Endast en bild
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 8 bitar, 4 komponenter, totalt 32
	desc.SampleDesc.Count = 1;	//Endast 1
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE; //kan inte ändras efter vi skapat den
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0; //kan inte komma åt efter den skapats - Vi gör inga ändringar med själva texturen efter den skapats
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &tData[0];
	data.SysMemPitch = tWidth * 4;	//Bredden, gånger 4 kanaler (RGBA), avståndet i byte för varje rad i texturen
	data.SysMemSlicePitch = 0;

	stbi_image_free(image);

	if (FAILED(device->CreateTexture2D(&desc, &data, &texture)))
	{

		std::cerr << "Failed to create texture!" << std::endl;
		return false;
	}

	HRESULT hr = device->CreateShaderResourceView(texture, nullptr, &textureSRV);
	return !FAILED(hr);
}


