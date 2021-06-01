#pragma once
#include "GameObject.h"
#include "Event.h"
#include <DirectXCollision.h>

class PlayerCamera :public GameObject
{
private:
	XMMATRIX perspectiveMatrix;
	XMMATRIX viewMatrix;
	XMVECTOR forward;
	XMVECTOR right;
	XMVECTOR up;

	XMVECTOR frustForward;
	XMVECTOR frustRight;

	float pitch;
	float yaw;
	float rotationSpeed;
	float speed;
	float viewDistance;

	float FOV;
	float aspectRatio;
	float nearZ;
	float farZ;

	float pickingDistance;
	XMVECTOR direction;

	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;
public:
	BoundingSphere boundingsphere;

public:
	PlayerCamera();
	PlayerCamera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float speed, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	float GetNearZ() const { return this->nearZ; };
	float GetFarZ() const { return this->farZ; };
	float GetRatio() const { return this->aspectRatio; };
	float GetFov() const { return this->FOV; };
	XMVECTOR GetFRightVector() const { return this->frustRight; };
	XMVECTOR GetUpVector() const { return this->up; };
	XMVECTOR GetFForwardVector() const { return this->frustForward; };
	XMMATRIX GetViewMatrix() const { return this->viewMatrix; };
	XMMATRIX GetPerspectiveMatrix() const { return this->perspectiveMatrix; };
	float getViewDistance();

	Type type() const override { return Type::CAMERA; };

	void MoveRight(float dt);
	void MoveForward(float dt);
	void Rotate(float dx, float dy);

	void PushBack(XMFLOAT3 direction, float dt);
	bool CheckCollision(BoundingOrientedBox& other);
	bool CheckCollision(BoundingSphere& other);

	bool CheckIntersection(BoundingOrientedBox& other);

	void OnEvent();
	void Update(float dt);
};