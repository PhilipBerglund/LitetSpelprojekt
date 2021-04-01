#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Geometry.h"

using namespace DirectX;

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	float specularPower;
	XMFLOAT3 padding;
	std::string textureName;
};

struct Mesh
{
	Material material;
	int vertexCount;

	std::vector<Polygon> polygons;
};

