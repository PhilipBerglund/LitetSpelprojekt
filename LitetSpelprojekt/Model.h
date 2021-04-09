#pragma once
#include "GameObject.h"
#include "Drawable.h"
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

class Model :public GameObject, Drawable
{
private:
	std::string name;
	int vertexCount;

	std::vector<Face> polygons;
	//Mesh* mesh;
	Material material;

	XMMATRIX worldMatrix;

	ID3D11ShaderResourceView* texture;
	ID3D11Buffer* vertexBuffer;

	bool LoadModel(std::string path);
	bool LoadTexture(std::string path);
public:
	Model();
	bool Initialize(ID3D11Device* device, std::string path);

	ID3D11ShaderResourceView* GetTexture();

	std::string GetName() const;
	XMMATRIX GetMatrix() const;
	int GetVertexCount() const;
	ID3D11Buffer* GetVertexBuffer();

	Type type() const override { return Type::MODEL; };

	Material GetMatrial() const;
	void SetMaterial(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, float specularPower);

	// Inherited via GameObject
	virtual void Update() override;

	// Inherited via Drawable
	virtual void UpdateBuffers(ID3D11DeviceContext* context) override;
};