#pragma once
#include "RenderPass.h"
#include "Scene.h"
#include "Shader.h"

class BaseRenderPass : public RenderPass
{
private:
	Shader shader;
public:
	BaseRenderPass(ID3D11Device& device, HWND window);
	// Inherited via RenderPass
	virtual void Execute(const Scene& scene, Graphics& graphics) override;
};