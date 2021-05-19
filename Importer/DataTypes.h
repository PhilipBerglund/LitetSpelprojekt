#pragma once

#include <d3d11.h>
#include <vector>
#include <iostream>

#include <wrl.h>
template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#define MAX_CHAR 100

enum class DataType
{
	UNKNOWN,
	VERTEXBUFFER,
	MESH,
	MATERIAL,
	GROUP,
	CAMERA,
	LIGHT,
	POINTLIGHT,
	SPOTLIGHT,
	DIRECTIONALLIGHT,
	SKELETON,
	ATTRIBUTE,
	MISC
};

struct Node
{
	DataType type = DataType::UNKNOWN;
	unsigned int sceneID = 0;
	unsigned int attributeCount = 0;
	//std::vector<Attribute> attributes;

	virtual ~Node() {};
};

struct Transform
{
	float translation[3] = {0};
	float rotation[4] = {0};
	float scale[3] = {0};
};

struct KeyFrame
{
	unsigned int timeStamp;
	Transform transform;
};

struct Animation
{
	char name[MAX_CHAR] = "";
	int keyFrameCount = 0;
	std::vector<KeyFrame> keyFrames;
};

struct Shape
{
	int vertexCount = 0;
	std::vector<std::pair<std::array<float, 4>, std::array<float, 3>>> transform;
};

struct MorphAnimation
{
	int shapeCount = 0;
	std::vector<Shape> shapes;
};

struct Skinning
{
	unsigned int weightCount = 0;
	unsigned int indexCount = 0;
	std::vector<float> weights;
	std::vector<unsigned int> indices;
};

struct Joint
{
	int parentID = -1;
	int ID = -1;
	char name[MAX_CHAR] = "";
	float inverseBP[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f };

	Animation animation;
	Skinning skinning;
};

struct Skeleton : Node
{
	int ID = -1;

	MorphAnimation morphAnim;

	unsigned int jointCount = 0;
	std::vector<Joint> joints;
};

enum class TextureType { UNKNOWN, DIFFUSE_COLOR, NORMAL_MAP, TRANSPARENT_COLOR };

struct Texture
{
	int ID = -1;
	TextureType type = TextureType::UNKNOWN;
	int width = 0;
	int height = 0;
	int fileSize = 0;
	char* data = nullptr;

	ID3D11ShaderResourceView** Get() 
	{	
		if (view.Get() == nullptr)
			return nullptr;

		return this->view.GetAddressOf(); 
	}

	bool Load(ID3D11Device& device)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
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
		data.pSysMem = this->data;
		data.SysMemPitch = width * 4;

		if (this->data)
		{
			ID3D11Texture2D* img;
			if FAILED(device.CreateTexture2D(&textureDesc, &data, &img))
			{
				std::cout << "FAILED TO CREATE TEXTURE 2D" << std::endl;
				return false;
			}

			if FAILED(device.CreateShaderResourceView(img, nullptr, &view))
			{
				std::cout << "FAILED TO CREATE SHADER RESOURCE VIEW" << std::endl;
				return false;
			}

			img->Release();
			img = nullptr;
		}

		return true;
	}

	Texture() = default;
private:
	ComPtr<ID3D11ShaderResourceView> view = nullptr;
};

struct Material :public Node
{
	int ID = -1;
	char name[MAX_CHAR] = "";
	float diffuse[3] = { 0 };
	float ambient[3] = { 0 };
	float emissive[3] = { 0 };
	float specular[3] = { 0 };

	float opacity = 0;
	float shininess = 0;
	float reflectivity = 0;

	unsigned int numTextures = 0;
	std::vector<Texture> textures;

	Material() = default;
};

struct Vertex
{
	float position[3] = { 0 };
	float normal[3] = { 0 };
	float uv[2] = { 0 };
	float tangent[3] = { 0 };
	float binormal[3] = { 0 };
	float weights[4] = { 0 };
	int boneIDs[4] = { -1 };
};

struct VertexBuffer
{
	int sceneID = 0;
	int ID = -1;
	unsigned int vertexCount = 0;
	std::vector<Vertex> vertices;
	ID3D11Buffer** GetBuffer() { return this->buffer.GetAddressOf(); }

	bool Initialize(ID3D11Device& device)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = vertexCount * sizeof(Vertex);
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(Vertex);

		Vertex* v = vertices.data();

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = vertices.data();

		if (FAILED(device.CreateBuffer(&desc, &data, &buffer)))
			return false;

		return true;
	}
private:
	ComPtr<ID3D11Buffer> buffer;
};

struct Mesh :public Node
{
	int ID = -1;
	char name[MAX_CHAR] = "";

	unsigned int materialCount = 0;

	int parentID = -1;
	int skeletonID = -1;
	int vertexBufferID = -1;

	float translation[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float scale[3] = { 0.0f, 0.0f, 0.0f };

	std::vector<int> materialIDs;

	Mesh() = default;
};

struct SceneData
{
	bool isInitialized = false;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<VertexBuffer> vertexBuffers;
	std::vector<Skeleton> skeletons;
};