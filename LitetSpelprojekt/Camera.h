#pragma once
#include "GameObject.h"
#include "Collider.h"

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

	SphereCollider collider;
public:
	Camera();
	Camera(float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	XMMATRIX GetViewMatrix() const { return this->viewMatrix; };
	XMMATRIX GetPerspectiveMatrix() const { return this->perspectiveMatrix; };

	Type type() const override { return Type::CAMERA; };

	void MoveRight(float dt);
	void MoveForward(float dt);
	void Rotate(float dx, float dy);

	void PushBack(XMFLOAT3 direction, float dt);
	bool CheckCollision(Collider& other);

	void Update();
};