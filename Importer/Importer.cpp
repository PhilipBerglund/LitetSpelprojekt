#include "Importer.h"
#include <iostream>
#include <fstream>

bool LoadTexture(Texture& texture, ID3D11Device& device)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = texture.width;
	textureDesc.Height = texture.height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = texture.data;
	data.SysMemPitch = texture.width * 4;

	if (texture.data)
	{
		ID3D11Texture2D* img;
		if FAILED(device.CreateTexture2D(&textureDesc, &data, &img))
		{
			std::cout << "FAILED TO CREATE TEXTURE 2D" << std::endl;
			return false;
		}

		if FAILED(device.CreateShaderResourceView(img, nullptr, texture.Get()))
		{
			std::cout << "FAILED TO CREATE SHADER RESOURCE VIEW" << std::endl;
			return false;
		}

		img->Release();
		img = nullptr;
	}

	return true;
}

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
			currentPosition += MAX_CHAR + sizeof(char);
		}

		void ReadTextureFile(char* data, int size)
		{
			std::memcpy(data, buffer.data() + currentPosition, size);
			currentPosition += size + sizeof(char);
		}

	public:
		void ReadScene(std::string path)
		{
			file.open(path, std::ios::in | std::ios::binary);

			if (!file.is_open())
				std::cerr << "COULD NOT OPEN FILE" << std::endl;

			buffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});
			SceneData scene;

			int header;
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

			Data::scenes.push_back(scene);
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
				mesh.materialIDs.push_back(ID);
			}

			mesh.sceneID = Data::scenes.size();
			scene.meshes.push_back(mesh);
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

				vertexBuffer.vertices.push_back(vertex);
			}

			vertexBuffer.sceneID = Data::scenes.size();
			scene.vertexBuffers.push_back(vertexBuffer);
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

				material.textures.push_back(texture);
			}

			material.sceneID = Data::scenes.size();
			scene.materials.push_back(material);
		}
	};

	void LoadScene(std::string path)
	{
		Reader reader;
		reader.ReadScene(path);
	}

	bool Initialize(ID3D11Device& device)
	{
		for (auto& scene : Data::scenes)
		{
			if (!scene.isInitialized)
			{
				for (auto& material : scene.materials)
				{
					for (auto& texture : material.textures)
					{
						if (!LoadTexture(texture, device))
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
		
		return true;
	}
}