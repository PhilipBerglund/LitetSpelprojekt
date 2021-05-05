#include "ShaderData.h"
#include "Graphics.h"
#include "ShaderLoader.h"

ShaderData::ShaderData()
{
	HRESULT hr;
	std::string byteCode;

	std::string vs_path;
	std::string ps_path;

	//-----PARTICLE SYSTEM-----
	//SHADERS
	if (!LoadVertexShader(particleVertexShader, vs_path, byteCode))
	{
		Error("FAILED TO CREATE VERTEX SHADER");
		return;
	}

	if (!LoadPixelShader(particlePixelShader, ps_path))
	{
		Error("FAILED TO CREATE PIXEL SHADER");
		return;
	}

	//INPUT LAYOUT
	
}

void ShaderData::Update()
{

}