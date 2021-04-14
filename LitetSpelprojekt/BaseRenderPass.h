#pragma once
#include "RenderPass.h"
#include "Shader.h"
#include "Scene.h"

class BaseRenderPass : public RenderPass
{
private:
	Shader shader;
public:
	BaseRenderPass(ID3D11Device& device, HWND window) { shader.Initialize(device, window); };
	
	// Inherited via RenderPass
	virtual void Execute(const Scene& scene, Graphics& graphics) override;
};