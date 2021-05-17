#pragma once
#include <string>
#include "DataTypes.h"

namespace Importer
{
	struct Data
	{
		static std::vector<SceneData> scenes;

		static std::vector<Mesh> GetMeshes(int sceneID) { return scenes.at(sceneID).meshes; }
		static Mesh& GetMeshAt(int sceneID, int index) { return scenes.at(sceneID).meshes.at(index); }
		static Material& GetMaterialAt(int sceneID, int index) { return scenes.at(sceneID).materials.at(index); }
		static VertexBuffer& GetVertexBufferAt(int sceneID, int index, bool) { return scenes.at(sceneID).vertexBuffers.at(index); }
		static ID3D11Buffer** GetVertexBufferAt(int sceneID, int index) { return scenes.at(sceneID).vertexBuffers.at(index).GetBuffer(); }
		static int GetVertexCountAt(int sceneID, int index) { return scenes.at(sceneID).vertexBuffers.at(index).vertexCount; }
	};

	void LoadScene(std::string path);
	bool Initialize(ID3D11Device& device);
}