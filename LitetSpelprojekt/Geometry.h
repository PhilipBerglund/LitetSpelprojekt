#pragma once
#include <array>
#include <string>
#include <vector>

struct Vertex
{
	float pos[3];
	float uv[2];
	float nrml[3];

	Vertex() = default;

	Vertex(const std::array<float, 3>& position, const std::array<float, 2>& uvCoords, const std::array<float, 3> normal)
	{
		for (int i = 0; i < 3; i++)
		{
			pos[i] = position[i];
			nrml[i] = normal[i];
		}

		uv[0] = uvCoords[0];
		uv[1] = uvCoords[1];
	}

	Vertex& operator=(const Vertex& vertex)
	{
		for (int i = 0; i < 3; ++i)
		{
			pos[i] = vertex.pos[i];
			nrml[i] = vertex.nrml[i];
		}

		uv[0] = vertex.uv[0];
		uv[1] = vertex.uv[1];

		return *this;
	}
};

struct Face
{
	Vertex vertices[3];

	Face() = default;

	Face(Vertex v1, Vertex v2, Vertex v3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
	}
};

struct Mesh
{
	Face test;
	int vertexCount;
	std::vector<Face> polygons;
};