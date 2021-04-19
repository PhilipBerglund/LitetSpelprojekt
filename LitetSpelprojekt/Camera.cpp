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
	:GameObject(position, rotation, scale), pitch(0), yaw(0), rotationSpeed(0.002f), speed(15.0f)
{
	this->up = { 0,1,0 };
	this->forward = { 0,0,1 };
	this->right = { 1,0,0 };
	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), forward, up);

	this->perspectiveMatrix = XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);

	this->direction = forward;
	this->boundingsphere = BoundingSphere(transform.position, 1);
	this->pickingDistance = 0.0001;
}

void Camera::MoveRight(float dt)
{
	XMVECTOR forwardVec =	XMVector3Transform(forward,
							XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
							XMMatrixScaling(speed, speed, speed));

	XMVECTOR rightVec = XMVector3Cross(up, forwardVec);
	rightVec = XMVector3Normalize(rightVec);
	XMFLOAT3 rightFloat;
	XMStoreFloat3(&rightFloat, rightVec);

	transform.position.x += rightFloat.x * dt * speed;
	//transform.position.y += rightFloat.y * dt * speed;
	transform.position.z += rightFloat.z * dt * speed;
}

void Camera::MoveForward(float dt)
{
	XMVECTOR forwardVec =	XMVector3Transform(forward,
							XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
							XMMatrixScaling(speed, speed, speed));
	forwardVec = XMVector3Normalize(forwardVec);
	XMFLOAT3 forwardFloat;
	XMStoreFloat3(&forwardFloat, forwardVec);

	transform.position.x += forwardFloat.x * dt * speed;
	//transform.position.y += forwardFloat.y * dt * speed;
	transform.position.z += forwardFloat.z * dt * speed;
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

void Camera::PushBack(XMFLOAT3 direction, float dt)
{
	transform.position.x += direction.x * dt;
	transform.position.z += direction.z * dt;
}

bool Camera::CheckCollision(BoundingOrientedBox& other)
{
	return boundingsphere.Intersects(other);
}

bool Camera::CheckCollision(BoundingSphere& other)
{
	return boundingsphere.Intersects(other);
}

bool Camera::CheckIntersection(BoundingOrientedBox& other)
{
	return other.Intersects(XMLoadFloat3(&transform.position), direction, pickingDistance);
}

void Camera::Update()
{
	const XMVECTOR lookAtVec = XMVector3Transform(forward, XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f));
	const XMVECTOR target = XMLoadFloat3(&transform.position) + lookAtVec;
	direction = XMVector3Normalize(lookAtVec);

	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), target, up);

	this->boundingsphere.Center.x = transform.position.x;
	this->boundingsphere.Center.y = transform.position.y;
	this->boundingsphere.Center.z = transform.position.z;
}