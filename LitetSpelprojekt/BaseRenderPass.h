#pragma once
#include "RenderPass.h"
#include "Scene.h"

class BaseRenderPass : public RenderPass
{
private:

public:
	// Inherited via RenderPass
	virtual void Execute(const Scene& scene, Graphics& graphics) override;
};