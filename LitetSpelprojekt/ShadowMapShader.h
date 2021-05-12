#pragma once
#include "ShaderData.h"
#include "ShadowMap.h"

class Scene;

class ShadowMapShader
{
private:
	void SetShadowMapShader(ShaderData& data);
public:
	void RenderShadowMap(ShaderData& data, Scene& scene);
};