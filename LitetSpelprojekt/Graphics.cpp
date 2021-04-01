#include "Graphics.h"
#include "PipelineHelper.h"

Graphics::Graphics()
{
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->layout = nullptr;
	this->sampler = nullptr;
}

bool Graphics::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if (!core.Initialize(windowWidth, windowHeight, window))
		return false;
	
	if (!SetupPipeline(core.GetDevice(), vertexShader, pixelShader, layout, sampler))
	{
		std::cerr << "Failed to setup pipeline" << std::endl;
		return false;
	}

	return true;
}

void Graphics::ShutDown()
{
	core.ShutDown();

	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}

	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}

	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}

	if (sampler)
	{
		sampler->Release();
		sampler = nullptr;
	}
}

void Graphics::Render()
{
	core.BeginScene(1, 1, 1);

	//RENDER STUFF

	core.EndScene();
}
