#pragma once
#include "ShaderData.h"

class Scene;

class ParticleShader
{
private:
	void SetRainShader(ShaderData& data);
	void SetSmokeShader(ShaderData& data);
public:
	void RenderRain(ShaderData& data, Scene& scene);
	void RenderSmoke(ShaderData& data, Scene& scene);
};