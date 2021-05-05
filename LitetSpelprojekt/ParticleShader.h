#pragma once
#include "Graphics.h"
#include "ShaderData.h"

class Scene;

class ParticleShader
{
private:
	void SetShader(ShaderData& data);
public:
	void Render(ShaderData& data, Scene& scene);
};