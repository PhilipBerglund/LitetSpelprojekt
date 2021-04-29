#include "Importer.h"
#include <iostream>

struct Data
{
	static std::vector<Mesh> meshes;

	static std::vector<Mesh> GetMeshes()
	{
		return meshes;
	}
};

std::vector<Mesh> Data::meshes;

#include <fstream>
class Reader
{
	std::ifstream file;
	int currentPosition;
	std::vector<unsigned char> buffer;

	template <typename T>
	void ReadVector(T vec[], int size, std::vector<unsigned char>& buffer)
	{
		for (int i = 0; i < size; ++i)
		{
			std::memcpy(&vec[i], buffer.data() + currentPosition, sizeof(T));
			currentPosition += sizeof(T);
		}
	}

	template <typename T>
	void Read(T& type, std::vector<unsigned char>& buffer)
	{
		std::memcpy(&type, buffer.data() + currentPosition, sizeof(T));
		currentPosition += sizeof(T);
	}

	void ReadName(char name[], std::vector<unsigned char>& buffer)
	{
		std::memcpy(name, buffer.data() + currentPosition, MAX_CHAR);
		currentPosition += MAX_CHAR + sizeof(char);
	}

public:
	void ReadScene(std::string path)
	{
		file.open(path, std::ios::in | std::ios::binary);

		if (!file.is_open())
			std::cerr << "COULD NOT OPEN FILE" << std::endl;

		buffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});

		int header;
		while (currentPosition < buffer.size())
		{
			std::memcpy(&header, buffer.data() + currentPosition, sizeof(int));
			currentPosition += sizeof(int);

			switch ((DataType)header)
			{
			case DataType::MESH:
				ReadMesh();
				break;
			}
		}
	}

	void ReadMesh()
	{
		Mesh mesh;

		Read(mesh.ID, buffer);
		ReadName(mesh.name, buffer);

		Read(mesh.vertexCount, buffer);
		Read(mesh.attributeCount, buffer);
		Read(mesh.materialCount, buffer);

		Read(mesh.materialID, buffer);
		Read(mesh.parentID, buffer);
		Read(mesh.skeletonID, buffer);

		ReadVector(mesh.translation, 3, buffer);
		ReadVector(mesh.rotation, 3, buffer);
		ReadVector(mesh.scale, 3, buffer);

		for (unsigned int i = 0; i < mesh.vertexCount; ++i)
		{
			Vertex vertex;

			ReadVector(vertex.position, 3, buffer);
			ReadVector(vertex.normal, 3, buffer);
			ReadVector(vertex.uv, 2, buffer);
			ReadVector(vertex.tangent, 3, buffer);
			ReadVector(vertex.binormal, 3, buffer);
			ReadVector(vertex.weights, 3, buffer);
			ReadVector(vertex.boneIDs, 4, buffer);

			mesh.vertices.push_back(vertex);
		}

		Data::meshes.push_back(mesh);
	}
};


namespace Importer
{
	void LoadScene(std::string path)
	{
		Reader reader;
		reader.ReadScene(path);
	}

	std::vector<Mesh> GetMeshes()
	{
		return Data::GetMeshes();
	}
}