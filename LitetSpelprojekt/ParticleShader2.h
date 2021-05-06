#pragma once
#include "ShaderData.h"

class Scene;

class ParticleShader2
{
private:
	void SetShader(ShaderData& data);
public:
	void Render(ShaderData& data, Scene& scene);
};