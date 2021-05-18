#pragma once
#include "GameObject.h"
#include "Event.h"
#include <DirectXCollision.h>

class Camera :public GameObject
{
private:
	XMMATRIX perspectiveMatrix;
	XMMATRIX viewMatrix;
	XMVECTOR forward;
	XMVECTOR right;
	XMVECTOR up;

	float pitch;
	float yaw;
	float rotationSpeed;
	float speed;

	float pickingDistance;
	XMVECTOR direction;

	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;
public:
	BoundingSphere boundingsphere;

public:
	Camera();
	Camera(float FOV, float aspectRatio, float nearZ, float farZ, float rotationSpeed, float speed, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	XMMATRIX GetViewMatrix() const { return this->viewMatrix; };
	XMMATRIX GetPerspectiveMatrix() const { return this->perspectiveMatrix; };

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