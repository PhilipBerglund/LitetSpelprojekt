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

	return true;
}

namespace Importer
{
	std::vector<Mesh> Data::meshes;
	std::vector<Material> Data::materials;
	std::vector<VertexBuffer> Data::vertexBuffers;

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

		void ReadName(char name[])
		{
			std::memcpy(name, buffer.data() + currentPosition, MAX_CHAR);
			currentPosition += MAX_CHAR + sizeof(char);
		}

		void ReadTextureFile(char data[])
		{
			std::memcpy(data, buffer.data() + currentPosition, MAX_TEXTURESIZE);
			currentPosition += MAX_TEXTURESIZE + sizeof(char);
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

				case DataType::MATERIAL:
					ReadMaterial();
					break;

				case DataType::VERTEXBUFFER:
					ReadVertexBuffer();
					break;
				}
			}
		}

	private:
		void ReadMesh()
		{
			Mesh mesh;

			Read(mesh.ID);
			ReadName(mesh.name);

			Read(mesh.materialCount);
			Read(mesh.attributeCount);

			Read(mesh.parentID);
			Read(mesh.skeletonID);
			Read(mesh.vertexBufferID);

			//ReadVector(mesh.matrix, 16);
			ReadVector(mesh.translation, 3);
			ReadVector(mesh.rotation, 4);
			ReadVector(mesh.scale, 3);

			for (unsigned int i = 0; i < mesh.materialCount; ++i)
			{
				int ID;
				Read(ID);
				mesh.materialIDs.push_back(ID);
			}

			Data::meshes.push_back(mesh);
		}

		void ReadVertexBuffer()
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

			Data::vertexBuffers.push_back(vertexBuffer);
		}

		void ReadMaterial()
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
				ReadTextureFile(texture.data);

				material.textures.push_back(texture);
			}

			Data::materials.push_back(material);
		}
	};

	void LoadScene(std::string path)
	{
		Reader reader;
		reader.ReadScene(path);
	}

	bool Initialize(ID3D11Device& device)
	{
		for (auto& material : Data::materials)
		{
			for (auto& texture : material.textures)
			{
				if (!LoadTexture(texture, device))
					return false;
			}
		}

		for (int i = 0; i < Data::vertexBuffers.size(); ++i)
		{
			if (!Data::vertexBuffers[i].Initialize(device))
				return false;
		}

		return true;
	}
}