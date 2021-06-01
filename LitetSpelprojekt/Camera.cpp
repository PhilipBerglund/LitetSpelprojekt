#include "Window.h"
#include "Camera.h"
#include <algorithm>
#include "GameSettings.h"
#include "Print.h"

PlayerCamera::PlayerCamera()
	:pitch(0), yaw(0), rotationSpeed(0), speed(0)
{
	Event::Bind(this, EventType::W_DOWN);
	Event::Bind(this, EventType::A_DOWN);
	Event::Bind(this, EventType::S_DOWN);
	Event::Bind(this, EventType::D_DOWN);
	Event::Bind(this, EventType::MOUSEMOVE);
	Event::Bind(this, EventType::RESET);

	this->up = { 0,1,0 };
	this->forward = { 0,0,1 };
	this->right = { 1,0,0 };
	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), forward, up);

	perspectiveMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)1024 / float(576), 0.1f, 100.0f);

	this->direction = forward;
	this->pickingDistance = 10.0f;
}

PlayerCamera::PlayerCamera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float speed, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
	:GameObject(position, rotation, scale), pitch(0), yaw(0), rotationSpeed(rotationSpeed), speed(speed)
{
	
	Event::Bind(this, EventType::W_DOWN);
	Event::Bind(this, EventType::A_DOWN);
	Event::Bind(this, EventType::S_DOWN);
	Event::Bind(this, EventType::D_DOWN);
	Event::Bind(this, EventType::MOUSEMOVE);
	Event::Bind(this, EventType::RESET);

	this->up = { 0,1,0 };
	this->forward = { 0, 0, 1 };
	this->right = { 1,0,0 };
	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), forward, up);
	this->viewDistance = farZ;

	this->perspectiveMatrix = XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);

	this->direction = forward;
	this->boundingsphere = BoundingSphere({ transform.position.x, 5, transform.position.z }, 1.0f);
	this->pickingDistance = 50.0f;

	this->FOV = FOV;
	this->aspectRatio = aspectRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;

	this->frustForward = { 0,0,1 };
	this->frustRight = { 1,0,0 };
}

float PlayerCamera::getViewDistance()
{
	return this->viewDistance;
}

void PlayerCamera::MoveRight(float dt)
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

void PlayerCamera::MoveForward(float dt)
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

void PlayerCamera::Rotate(float dx, float dy)
{
	yaw = (yaw + dx * rotationSpeed);
	yaw = (float)fmod((double)yaw + XM_PI, XM_2PI);
	if (yaw < 0)
		yaw += XM_2PI;
	yaw -= XM_PI;

	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -XM_PIDIV2, 0.995f * XM_PIDIV2);
}

void PlayerCamera::PushBack(XMFLOAT3 direction, float dt)
{
	transform.position.x += direction.x * dt;
	transform.position.z += direction.z * dt;
}

bool PlayerCamera::CheckCollision(BoundingOrientedBox& other)
{
	return boundingsphere.Intersects(other);
}

bool PlayerCamera::CheckCollision(BoundingSphere& other)
{
	return boundingsphere.Intersects(other);
}

bool PlayerCamera::CheckIntersection(BoundingOrientedBox& other)
{
	XMFLOAT3 distVec = {	transform.position.x - other.Center.x,
							transform.position.y - other.Center.y,
							transform.position.z - other.Center.z	};

	float distance = sqrt(pow(distVec.x,2) + pow(distVec.y,2) + pow(distVec.z,2));

	float temp;
	if (distance <= pickingDistance)
		return other.Intersects(XMLoadFloat3(&transform.position), direction, temp);

	return false;
}

void PlayerCamera::OnEvent()
{
	if (GameSettings::GetState() != GameState::INGAME)
		return;

	switch(Event::GetCurrentEvent())
	{
	case EventType::RESET:
		w = false;
		a = false;
		s = false;
		d = false;
		break;

	case EventType::W_DOWN:
		w = true;
		break;

	case EventType::A_DOWN:
		a = true;
		break;

	case EventType::S_DOWN:
		s = true;
		break;

	case EventType::D_DOWN:
		d = true;
		break;

	case EventType::W_UP:
		w = false;
		break;

	case EventType::A_UP:
		a = false;
		break;

	case EventType::S_UP:
		s = false;
		break;

	case EventType::D_UP:
		d = false;
		break;

	case EventType::MOUSEMOVE:
		std::pair<float, float> delta = Window::GetRawInput().value();
		Rotate(delta.first, delta.second);
		break;
	}
}

void PlayerCamera::Update(float dt)
{
	if (w)
		MoveForward(dt);

	if (a)
		MoveRight(-dt);

	if (s)
		MoveForward(-dt);

	if (d)
		MoveRight(dt);

	const XMVECTOR lookAtVec = XMVector3Transform(forward, XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f));
	const XMVECTOR target = XMLoadFloat3(&transform.position) + lookAtVec;
	direction = XMVector3Normalize(lookAtVec);

	this->viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&transform.position), target, up);

	this->boundingsphere.Center.x = transform.position.x;
	this->boundingsphere.Center.y = transform.position.y;
	this->boundingsphere.Center.z = transform.position.z;

	this->frustForward = lookAtVec;
	this->frustRight = XMVector3Cross(up, lookAtVec);
}