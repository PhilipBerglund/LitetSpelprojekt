#include "Light.h"

ShadowLight::ShadowLight()
{
    attributes.ambient = { 1,1,1,1 };
    attributes.diffuse = { 1,1,1,1 };
  
    XMFLOAT3 up = { 0,1,0 };
    this->forward = { -1,0,1 };
    this->transform.position = { 0,1.4,0 };
    this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), XMLoadFloat3(&forward), XMLoadFloat3(&up));
    this->shadowViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), { 0,0,0 }, XMLoadFloat3(&up));
    this->orthographicMatrix = XMMatrixOrthographicOffCenterLH(-600.0f, 600.0f, -400, 600.0f, -800.0f, 500.0f);

    perspectiveMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)1024 / float(476), 0.1, 100);
}

ShadowLight::ShadowLight(XMFLOAT4 ambient, XMFLOAT4 diffuse, float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
    :GameObject(position, rotation, scale)
{
    attributes.ambient = ambient;
    attributes.diffuse = diffuse;

    XMFLOAT3 up = { 0,1,0 };
    this->forward = { 0,0,1 };
    this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), XMLoadFloat3(&forward), XMLoadFloat3(&up));
    this->shadowViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), { 0,0,0 }, XMLoadFloat3(&up));
    this->orthographicMatrix = XMMatrixOrthographicOffCenterLH(-600.0f, 600.0f, -600, 600.0f, 0.1f, 700.0f);

    perspectiveMatrix = XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);
}