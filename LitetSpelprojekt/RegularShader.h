#pragma once
#include "ShaderData.h"

class Scene;
class Model;

class RegularShader
{
private:
	void SetShader(ShaderData& data);
	void UpdatePerMesh(ShaderData& data, Model& model);
public:
	void Render(ShaderData& data, Scene& scene);
};