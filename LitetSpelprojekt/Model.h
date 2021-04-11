#pragma once
#include "GameObject.h"
#include "Geometry.h"

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

	std::vector<Face> polygons;
	Material material;

	XMMATRIX worldMatrix;

	ComPtr<ID3D11ShaderResourceView> texture;
	ComPtr<ID3D11Buffer> vertexBuffer;

	bool LoadModel(std::string path);
	bool LoadTexture(std::string path);
public:
	Model();
	bool Initialize(ID3D11Device& device, std::string path);

	ID3D11ShaderResourceView& GetTexture();

	std::string GetName() const;
	XMMATRIX GetMatrix() const;
	int GetVertexCount() const;
	ID3D11Buffer& GetVertexBuffer();

	Type type() const override { return Type::MODEL; };

	Material GetMatrial() const;
	void SetMaterial(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, float specularPower);

	void Update(ID3D11DeviceContext& context);
};