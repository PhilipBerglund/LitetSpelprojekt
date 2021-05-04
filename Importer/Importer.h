#pragma once
#include <string>
#include "DataTypes.h"

namespace Importer
{
	struct Data
	{
		friend class Reader;

		static std::vector<Mesh> GetMeshes() { return meshes; }
		static std::vector<VertexBuffer> GetVertexBuffers() { return vertexBuffers; }
		static Mesh& GetMeshAt(int index) { return meshes.at(index); }
		static Material& GetMaterialAt(int index) { return materials.at(index); }
		static ID3D11Buffer& GetVertexBufferAt(int index) { return vertexBuffers.at(index).GetBuffer(); }
		static int GetVertexCountAt(int index) { return vertexBuffers.at(index).vertexCount; }

		static std::vector<Mesh> meshes;
		static std::vector<Material> materials;
		static std::vector<VertexBuffer> vertexBuffers;
	};

	void LoadScene(std::string path);
	bool Initialize(ID3D11Device& device);
}