#pragma once
#include <string>
#include "DataTypes.h"

namespace Importer
{
	struct Data
	{
		//All scenes loaded into game
		static std::vector<SceneData> scenes;

		//Get all meshes in specified scene
		static std::vector<Mesh> GetMeshes(int sceneID) { return scenes.at(sceneID).meshes; }

		//Get mesh at specific index in specified scene
		static Mesh& GetMeshAt(int sceneID, int index) { return scenes.at(sceneID).meshes.at(index); }

		//Get material at specific index in specified scene
		static Material& GetMaterialAt(int sceneID, int index) { return scenes.at(sceneID).materials.at(index); }

		//Get vertex buffer STRUCT at specific index in specified scene
		static VertexBuffer& GetVertexBufferAt(int sceneID, int index, bool) { return scenes.at(sceneID).vertexBuffers.at(index); }

		//Get vertex buffer (ID3D11Buffer) at specific index in specified scene
		static ID3D11Buffer** GetVertexBufferAt(int sceneID, int index) { return scenes.at(sceneID).vertexBuffers.at(index).GetBuffer(); }

		//Get vertex count at specific index in specified scene
		static int GetVertexCountAt(int sceneID, int index) { return scenes.at(sceneID).vertexBuffers.at(index).vertexCount; }
	};

	//Load scene into game (.mff format)
	void LoadScene(std::string path);

	//Initialize any buffers or textures in ALL scenes, already initialized scenes will be skipped
	bool Initialize(ID3D11Device& device);
}