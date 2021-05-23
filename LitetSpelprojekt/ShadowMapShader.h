#pragma once
#include "ShaderData.h"
#include "ShadowMap.h"
#include "Light.h"

class Scene;
class Model;

class ShadowMapShader
{
private:
	void SetShadowMapShader(ShaderData& data);
	void UpdatePerMesh(ShaderData& data, Model& model);
public:
	ShadowMapShader() = default;
	void RenderShadowMap(ShaderData& data, Scene& scene);
};