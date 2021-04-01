#include "Light.h"

Light::Light()
{
    attributes.ambient = { 1,1,1,1 };
    attributes.diffuse = { 1,1,1,1 };
  
    XMFLOAT3 up = { 0,1,0 };
    this->forward = { 0,0,1 };
    this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), XMLoadFloat3(&forward), XMLoadFloat3(&up));

    perspectiveMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)1024 / float(476), 0.1, 100);
}

Light::Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
    :GameObject(position, rotation, scale)
{
    attributes.ambient = ambient;
    attributes.diffuse = diffuse;

    XMFLOAT3 up = { 0,1,0 };
    this->forward = { 0,0,1 };
    this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), XMLoadFloat3(&forward), XMLoadFloat3(&up));

    perspectiveMatrix = XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);
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
