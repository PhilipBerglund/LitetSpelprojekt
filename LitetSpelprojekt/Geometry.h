#pragma once
#include <array>
#include <string>
#include <vector>
#include <fstream>

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

//struct Mesh
//{
//	std::vector<Face> polygons;
//
//	bool Load(std::string path, int & vertexCount)
//	{
//	std::vector<std::array<float, 3>> v;
//	std::vector<std::array<float, 2>> vt;
//	std::vector<std::array<float, 3>> vn;
//	std::vector<std::array<std::array<int, 3>, 3>> f;
//
//	std::ifstream reader;
//
//	reader.open(path, std::ios::beg);
//
//	if (!reader.is_open())
//		return false;
//
//	int counter = 0;
//	std::string line;
//	while (std::getline(reader, line))
//	{
//		std::string text;
//		reader >> text;
//		if (text == "v")
//		{
//			std::array<float, 3> vertex;
//
//			reader >> vertex[0];
//			reader >> vertex[1];
//			reader >> vertex[2];
//
//			v.push_back(vertex);
//		}
//
//		if (text == "vt")
//		{
//			std::array<float, 2> texCoord;
//
//			reader >> texCoord[0];
//			reader >> texCoord[1];
//
//			texCoord[1] = 1 - texCoord[1];
//
//			vt.push_back(texCoord);
//		}
//
//		if (text == "vn")
//		{
//			std::array<float, 3> normal;
//
//			reader >> normal[0];
//			reader >> normal[1];
//			reader >> normal[2];
//
//			vn.push_back(normal);
//		}
//
//		if (text == "f")
//		{
//			std::array<std::array<int, 3>, 3> face;
//
//			std::string temp;
//
//			for (int i = 0; i < 3; ++i)
//			{
//				reader >> temp;
//
//				std::string substr;
//
//				face[0][i] = std::stoi(temp.substr(0, temp.find("/")));
//
//				substr = temp.substr(temp.find("/") + 1, temp.length());
//				face[1][i] = std::stoi(substr.substr(0, substr.find("/")));
//				face[2][i] = std::stoi(substr.substr(substr.find("/") + 1, substr.length()));
//			}
//
//			f.push_back(face);
//		}
//	}
//
//	for (int i = 0; i < f.size(); ++i)
//	{
//		std::array<float, 3> pos;
//		std::array<float, 2> tex;
//		std::array<float, 3> nor;
//		std::array<Vertex, 3> vertices;
//
//		for (int j = 0; j < 3; ++j)
//		{
//			int index = f[i][0][j] - 1;
//
//			pos[0] = v[index][0];
//			pos[1] = v[index][1];
//			pos[2] = v[index][2];
//
//			index = f[i][1][j] - 1;
//
//			tex[0] = vt[index][0];
//			tex[1] = vt[index][1];
//
//			index = f[i][2][j] - 1;
//
//			nor[0] = vn[index][0];
//			nor[1] = vn[index][1];
//			nor[2] = vn[index][2];
//
//			vertices[j] = Vertex(pos, tex, nor);
//			vertexCount++;
//		}
//
//		Face face = Face(vertices[0], vertices[1], vertices[2]);
//		polygons.push_back(face);
//	}
//
//	return true;
//	}
//};