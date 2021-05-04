#pragma once
#include "Importer.h"
#include "GameObject.h"
#include "Geometry.h"
#include <DirectXCollision.h>
#include "Graphics.h"

enum class ColliderType {BOX, SPHERE};

class Model :public GameObject
{
private:
	std::string name;
	Mesh mesh;
	XMMATRIX worldMatrix;
public:
	ColliderType collidertype = ColliderType::BOX;
	bool isInteractable = true;
	BoundingOrientedBox  boundingbox;

	Model(const Mesh& mesh);
	Model();
	void Update(ID3D11DeviceContext& context);

	Type type() const override { return Type::MODEL; };

	ID3D11Buffer& GetVertexBuffer() const	{ return Importer::Data::GetVertexBufferAt(mesh.vertexBufferID); };
	std::string GetName() const				{ return this->name; };
	XMMATRIX GetMatrix() const				{ return this->worldMatrix; };
	int GetVertexCount() const				{ return Importer::Data::GetVertexCountAt(mesh.vertexBufferID); };
	
	ID3D11ShaderResourceView** GetDiffuseTexture() 
	{  
		for (auto& ID : mesh.materialIDs)
		{
			for (auto& texture : Importer::Data::GetMaterialAt(ID).textures)
			{
				if (texture.type == TextureType::DIFFUSE_COLOR)
					return texture.Get();
			}
		}
		
		Texture nulltexture;
		return nulltexture.Get();
	}

	Material GetMaterial() const				{ return Importer::Data::GetMaterialAt(mesh.materialIDs[0]); };
};