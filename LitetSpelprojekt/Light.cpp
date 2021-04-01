#include "Light.h"

Light::Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
    :GameObject(position, rotation, scale)
{
    attributes.ambient = ambient;
    attributes.diffuse = diffuse;
    
    perspectiveMatrix = XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);
    viewMatrix = XMMatrixIdentity();
}

XMMATRIX Light::GetViewMatrix() const
{
    return this->viewMatrix;
}

XMMATRIX Light::GetPerspectiveMatrix() const
{
    return this->perspectiveMatrix;
}

LightAttributes Light::GetAttributes() const
{
    return this->attributes;
}

void Light::Update()
{
}

void Light::UpdateBuffers(ID3D11DeviceContext* context)
{
}
