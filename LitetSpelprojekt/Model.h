#pragma once
#include "Importer.h"
#include "GameObject.h"
#include <DirectXCollision.h>
#include "Graphics.h"
#include <map>

enum class ColliderType {BOX, SPHERE};

class Model :public GameObject
{
private:
	Mesh mesh;
	std::string name;
	int currentKeyFrame;
	int keyFrames;

	XMMATRIX worldMatrix;
	std::vector<std::map<int, XMFLOAT4X4>> jointAnim;

	void GetJointMatrix(Skeleton& skeleton, int jointID, int keyFrame, XMMATRIX matrix);
	void GetJointMatrix(Skeleton& skeleton, int jointID, int keyFrame, XMVECTOR& scale, XMVECTOR& translation, XMVECTOR& rotation);

public:
	ColliderType collidertype = ColliderType::BOX;
	bool isInteractable = true;
	BoundingOrientedBox  boundingbox;

	Model(const Mesh& mesh);
	Model();
	void Update(ID3D11DeviceContext& context);

	Type type() const override { return Type::MODEL; };

	ID3D11Buffer** GetVertexBuffer() const	{ return Importer::Data::GetVertexBufferAt(mesh.sceneID, mesh.vertexBufferID); };
	std::string GetName() const				{ return this->name; };
	void SetName(std::string name)			{ this->name = name; }
	XMMATRIX GetMatrix() const				{ return this->worldMatrix; };
	int GetVertexCount() const				{ return Importer::Data::GetVertexCountAt(mesh.sceneID, mesh.vertexBufferID); };
	
	ID3D11ShaderResourceView** GetDiffuseTexture() 
	{  
		for (auto& ID : mesh.materialIDs)
		{
			for (auto& texture : Importer::Data::GetMaterialAt(mesh.sceneID, ID).textures)
			{
				if (texture.type == TextureType::DIFFUSE_COLOR)
					return texture.Get();
			}
		}
		
		return nullptr;
	}

	Material GetMaterial() const				{ return Importer::Data::GetMaterialAt(mesh.sceneID, mesh.materialIDs[0]); };

	//std::vector<XMMATRIX> GetJointTransforms() { return this->jointTransforms; }

	int GetCurrentKeyFrame() const { return currentKeyFrame; }
	int GetNumJoints() const { return jointAnim.size(); };
	int GetNumKeyFrames() const { return keyFrames; }
	XMFLOAT4X4 GetJointTransform(int ID, int keyFrame) { return jointAnim[ID].at(keyFrame); }
	void UpdateAnimation(float time);
};