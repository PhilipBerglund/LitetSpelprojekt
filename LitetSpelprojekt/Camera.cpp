#include "Camera.h"
#include <algorithm>

Camera::Camera()
	:pitch(0), yaw(0), rotationSpeed(0), speed(0)
{
	XMFLOAT3 up = { 0,1,0 };
	this->forward = { 0,0,1 };
	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), XMLoadFloat3(&forward), XMLoadFloat3(&up));

	perspectiveMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)1024 / float(576), 0.1, 100);
}

Camera::Camera(float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
	:GameObject(position, rotation, scale), pitch(0), yaw(0), rotationSpeed(0), speed(0)
{
	XMFLOAT3 up = { 0,1,0 };
	this->forward = { 0,0,1 };
	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), XMLoadFloat3(&forward), XMLoadFloat3(&up));

	this->perspectiveMatrix = XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);
}

XMMATRIX Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

XMMATRIX Camera::GetPerspectiveMatrix() const
{
	return this->perspectiveMatrix;
}

template <typename T>
T clamp(T x, T min, T max)
{
	if (x < min) x = min;
	if (x > max) x = max;
	return x;
}

void Camera::Rotate(float dx, float dy)
{
	yaw = (yaw + dx * rotationSpeed);
	yaw = fmod((double)yaw + XM_PI, XM_2PI);
	if (yaw < 0)
		yaw += XM_2PI;
	yaw -= XM_PI;

	pitch = clamp(pitch + dy * rotationSpeed, 0.995f * -XM_PIDIV2, 0.995f / XM_PIDIV2);
}

void Camera::Update()
{
	XMFLOAT3 up = { 0,1,0 };
	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), XMLoadFloat3(&forward), XMLoadFloat3(&up));
}
