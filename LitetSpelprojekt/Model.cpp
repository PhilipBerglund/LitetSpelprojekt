#include "Model.h"

void Model::Update()
{
}

void Model::UpdateBuffers(ID3D11DeviceContext* context)
{
}

bool Model::LoadModel(std::string path)
{
    return false;
}

bool Model::LoadTexture(std::string path)
{
    return false;
}

bool Model::Initialize(ID3D11Device* device, std::string path)
{
    if (!LoadModel(path))
        return false;

    if (!LoadTexture(path))
        return false;

    return true;
}

Model::Model()
{
}

ID3D11ShaderResourceView* Model::GetTexture()
{
    return this->texture;
}

XMMATRIX Model::GetMatrix() const
{
    return this->worldMatrix;
}

Material Model::GetMatrial() const
{
    return this->material;
}

void Model::SetMaterial(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, float specularPower)
{
    this->material.ambient = ambient;
    this->material.diffuse = diffuse;
    this->material.specular = specular;
    this->material.specularPower = specularPower;
}