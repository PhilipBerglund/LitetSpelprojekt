#include "Camera.h"
#include <algorithm>

Camera::Camera()
	:pitch(0), yaw(0), rotationSpeed(0), speed(0)
{
	this->up = { 0,1,0 };
	this->forward = { 0,0,1 };
	this->right = { 1,0,0 };
	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), forward, up);

	perspectiveMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)1024 / float(576), 0.1, 100);
}

Camera::Camera(float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
	:GameObject(position, rotation, scale), pitch(0), yaw(0), rotationSpeed(0.004f), speed(15.0f)
{
	this->up = { 0,1,0 };
	this->forward = { 0,0,1 };
	this->right = { 1,0,0 };
	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), forward, up);

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

float Camera::GetSpeed() const
{
	return this->speed;
}

void Camera::MoveRight(float dt)
{
	XMVECTOR forwardVec =	XMVector3Transform(forward,
							XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
							XMMatrixScaling(speed, speed, speed));

	XMVECTOR rightVec = XMVector3Cross(up, forwardVec);
	XMFLOAT3 rightFloat;
	XMStoreFloat3(&rightFloat, rightVec);
	transform.position.x += rightFloat.x * dt;
	//transform.position.y += rightFloat.y * dt;
	transform.position.z += rightFloat.z * dt;
}

void Camera::MoveForward(float dt)
{
	XMVECTOR forwardVec =	XMVector3Transform(forward,
							XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
							XMMatrixScaling(speed, speed, speed));

	XMFLOAT3 forwardFloat;
	XMStoreFloat3(&forwardFloat, forwardVec);

	transform.position.x += forwardFloat.x * dt;
	//transform.position.y += forwardFloat.y * dt;
	transform.position.z += forwardFloat.z * dt;
}

void Camera::Rotate(float dx, float dy)
{
	yaw = (yaw + dx * rotationSpeed);
	yaw = fmod((double)yaw + XM_PI, XM_2PI);
	if (yaw < 0)
		yaw += XM_2PI;
	yaw -= XM_PI;

	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -XM_PIDIV2, 0.995f * XM_PIDIV2);
}

void Camera::Update()
{
	const XMVECTOR lookAtVec = XMVector3Transform(forward, XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f));
	const XMVECTOR target = XMLoadFloat3(&transform.position) + lookAtVec;

	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), target, up);
}