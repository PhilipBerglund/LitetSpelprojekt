#pragma once
#include "RenderPass.h"

class ParticleRenderPass : public RenderPass
{
private:

public:
	// Inherited via RenderPass
	virtual void Execute(const Scene& scene, Graphics& graphics) override;
};
