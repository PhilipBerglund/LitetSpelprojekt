#include "Camera.h"

Camera::Camera(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
	:GameObject(position, rotation, scale)
{
	viewMatrix = XMMatrixIdentity();
}

XMMATRIX Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

void Camera::Update()
{
}
