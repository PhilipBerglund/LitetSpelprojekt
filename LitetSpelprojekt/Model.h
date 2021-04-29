#pragma once
#include "Importer.h"
#include "GameObject.h"
#include "Geometry.h"
#include <DirectXCollision.h>
#include "Graphics.h"

enum class ColliderType {BOX, SPHERE};

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
	float specularPower;
	XMFLOAT3 padding;
	std::string textureName;
};

class Model :public GameObject
{
private:
	std::string name;
	int vertexCount;

	Mesh mesh;

	std::vector<Face> polygons;
	Material material;

	XMMATRIX worldMatrix;

	ComPtr<ID3D11ShaderResourceView> texture;
	ComPtr<ID3D11Buffer> vertexBuffer;

	bool LoadModel(std::string path);
	bool LoadTexture(std::string path);
public:
	ColliderType collidertype = ColliderType::BOX;
	bool isInteractable = true;
	BoundingOrientedBox  boundingbox;

	Model(const Mesh& mesh);
	Model();
	void Update(ID3D11DeviceContext& context);
	bool Initialize();
	bool Initialize(ID3D11Device& device, std::string path);

	Type type() const override { return Type::MODEL; };

	ID3D11Buffer& GetVertexBuffer() const	{ return *this->mesh.vertexBuffer.Get(); };
	ID3D11ShaderResourceView& GetTexture()	{ return *this->texture.Get(); };
	std::string GetName() const				{ return this->name; };
	XMMATRIX GetMatrix() const				{ return this->worldMatrix; };
	int GetVertexCount() const				{ return this->mesh.vertexCount; };
	Material GetMatrial() const				{ return this->material; };

	void SetMaterial(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, float specularPower);
};