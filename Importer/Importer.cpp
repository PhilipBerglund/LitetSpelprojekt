#include "Importer.h"
#include <iostream>
#include <fstream>
#include "TempTimer.h"

namespace Importer
{
	std::vector<SceneData> Data::scenes;

	class Reader
	{
		std::ifstream file;
		int currentPosition = 0;
		std::vector<unsigned char> buffer;

		template <typename T>
		void ReadVector(T vec[], int size)
		{
			for (int i = 0; i < size; ++i)
			{
				std::memcpy(&vec[i], buffer.data() + currentPosition, sizeof(T));
				currentPosition += sizeof(T);
			}
		}

		template <typename T>
		void Read(T& type)
		{
			std::memcpy(&type, buffer.data() + currentPosition, sizeof(T));
			currentPosition += sizeof(T);
		}

		void ReadName(char* name)
		{
			std::memcpy(name, buffer.data() + currentPosition, MAX_CHAR);
			currentPosition += MAX_CHAR;
		}

		void ReadTextureFile(char* data, int size)
		{
			std::memcpy(data, buffer.data() + currentPosition, size);
			currentPosition += size;
		}

	public:
		void ReadScene(std::string path)
		{
			TempTimer tempTimer;

			file.open(path, std::ios::in | std::ios::binary);

			if (!file.is_open())
				std::cerr << "COULD NOT OPEN FILE" << std::endl;

			tempTimer.Start();

			buffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});
			std::cout << "LOAD FILE DATA: " << tempTimer.DeltaTime() << std::endl;

			SceneData scene;

			int header;
			tempTimer.Start();
			while (currentPosition < buffer.size())
			{
				std::memcpy(&header, buffer.data() + currentPosition, sizeof(int));
				currentPosition += sizeof(int);

				switch ((DataType)header)
				{
				case DataType::MESH:
					ReadMesh(scene);
					break;

				case DataType::MATERIAL:
					ReadMaterial(scene);
					break;

				case DataType::VERTEXBUFFER:
					ReadVertexBuffer(scene);
					break;
				}
			}
			std::cout << "ACTUALLY READ DATA: " << tempTimer.DeltaTime() << std::endl;

			Data::scenes.emplace_back(scene);
			file.close();
		}

	private:
		void ReadMesh(SceneData& scene)
		{
			Mesh mesh;
			
			Read(mesh.ID);
			ReadName(mesh.name);

			Read(mesh.materialCount);
			Read(mesh.attributeCount);

			Read(mesh.parentID);
			Read(mesh.skeletonID);
			Read(mesh.vertexBufferID);

			ReadVector(mesh.translation, 3);
			ReadVector(mesh.rotation, 4);
			ReadVector(mesh.scale, 3);

			for (unsigned int i = 0; i < mesh.materialCount; ++i)
			{
				int ID;
				Read(ID);
				mesh.materialIDs.emplace_back(ID);
			}

			mesh.sceneID = Data::scenes.size();
			scene.meshes.emplace_back(mesh);
		}

		void ReadVertexBuffer(SceneData& scene)
		{
			VertexBuffer vertexBuffer;

			Read(vertexBuffer.ID);
			Read(vertexBuffer.vertexCount);

			for (int i = 0; i < vertexBuffer.vertexCount; ++i)
			{
				Vertex vertex;

				ReadVector(vertex.position, 3);
				ReadVector(vertex.normal, 3);
				ReadVector(vertex.uv, 2);
				ReadVector(vertex.tangent, 3);
				ReadVector(vertex.binormal, 3);
				ReadVector(vertex.weights, 3);
				ReadVector(vertex.boneIDs, 4);

				vertexBuffer.vertices.emplace_back(vertex);
			}

			vertexBuffer.sceneID = Data::scenes.size();
			scene.vertexBuffers.emplace_back(vertexBuffer);
		}

		void ReadMaterial(SceneData& scene)
		{
			Material material;

			Read(material.ID);
			ReadName(material.name);
			ReadVector(material.diffuse, 3);
			ReadVector(material.ambient, 3);
			ReadVector(material.emissive, 3);
			ReadVector(material.specular, 3);
			Read(material.opacity);
			Read(material.shininess);
			Read(material.reflectivity);

			Read(material.numTextures);
			for (int i = 0; i < material.numTextures; ++i)
			{
				Texture texture;

				Read(texture.ID);
				Read(texture.type);
				Read(texture.width);
				Read(texture.height);
				Read(texture.fileSize);

				texture.data = (char*)malloc(texture.fileSize);
				ReadTextureFile(texture.data, texture.fileSize);

				material.textures.emplace_back(texture);
			}

			material.sceneID = Data::scenes.size();
			scene.materials.emplace_back(material);
		}
	};

	void LoadScene(std::string path)
	{
		Reader reader;
		reader.ReadScene(path);
	}

	bool Initialize(ID3D11Device& device)
	{
		TempTimer temp;
		temp.Start();

		for (auto& scene : Data::scenes)
		{
			if (!scene.isInitialized)
			{
				for (auto& material : scene.materials)
				{
					for (auto& texture : material.textures)
					{
						if (!texture.Load(device))
							return false;
					}
				}

				for (int i = 0; i < scene.vertexBuffers.size(); ++i)
				{
					if (!scene.vertexBuffers[i].Initialize(device))
						return false;
				}

				scene.isInitialized = true;
			}
		}

		std::cout << "INIT TIME: " << temp.DeltaTime() << std::endl;
		
		return true;
	}
}