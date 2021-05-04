#pragma once

#include <d3d11.h>
#include <vector>

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
	TEXTURE,
	MATERIAL_TEXTURE_CONNECTION,
	GROUP,
	CAMERA,
	LIGHT,
	POINTLIGHT,
	SPOTLIGHT,
	DIRECTIONALLIGHT,
	JOINT,
	SKELETON,
	ATTRIBUTE,
	SKINNING,
	KEYFRAME,
	MISC
};

//enum class AttributeType { UNKNOWN, VECTOR3, VECTOR4, INTEGER, STRING, FLOAT, BOOLEAN, DOUBLE, ENUM };
//
//struct Value
//{};
//
//struct Boolean : Value
//{
//	bool value = false;
//};
//
//struct Integer : Value
//{
//	int value = 0;
//};
//
//struct Float : Value
//{
//	float value = 0.0f;
//};
//
//struct Double : Value
//{
//	double value = 0.0;
//};
//
//struct String : Value
//{
//	char value[MAX_CHAR] = "";
//};
//
//struct Vector3 : Value
//{
//	double x = 0;
//	double y = 0;
//	double z = 0;
//};
//
//struct Vector4 : Value
//{
//	double x = 0;
//	double y = 0;
//	double z = 0;
//	double w = 0;
//};
//
//struct Attribute
//{
//	int ID = -1;
//	char name[MAX_CHAR] = "";
//
//	bool isAnimateable = false;
//
//	bool hasMaxLimit = false;
//	double maxLimit = 0;
//
//	bool hasMinLimit = false;
//	double minLimit = 0;
//
//	AttributeType type = AttributeType::UNKNOWN;
//	Value value;
//
//	Attribute() = default;
//};

struct Node
{
	DataType type = DataType::UNKNOWN;

	unsigned int attributeCount = 0;
	//std::vector<Attribute> attributes;

	virtual ~Node() {};
};

enum class TextureType { UNKNOWN, DIFFUSE_COLOR, NORMAL_MAP, TRANSPARENT_COLOR };

struct Texture
{
	int ID = -1;
	TextureType type = TextureType::UNKNOWN;
	char path[MAX_CHAR] = "";

	ID3D11ShaderResourceView** Get() { return this->view.GetAddressOf(); }
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
	float weights[3] = { 0 };
	int boneIDs[4] = { -1 };
};

struct VertexBuffer
{
	int ID = -1;
	unsigned int vertexCount = 0;
	std::vector<Vertex> vertices;
	ID3D11Buffer& GetBuffer() {	return *this->buffer.Get();	}

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
	//float matrix[16] = { 1, 0, 0, 0,
	//					0, 1, 0, 0,
	//					0, 0, 1, 0,
	//					0, 0, 0, 1 };

	float translation[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float scale[3] = { 0.0f, 0.0f, 0.0f };

	std::vector<int> materialIDs;

	Mesh() = default;
	/*bool Initialize(ID3D11Device& device)
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

		if (FAILED(device.CreateBuffer(&desc, &data, &vertexBuffer)))
			return false;

		return true;
	}*/
};