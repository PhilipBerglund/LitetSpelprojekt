#pragma once
#include "GameObject.h"

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
public:
	Camera();
	Camera(float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetPerspectiveMatrix() const;
	float GetSpeed() const;

	Type type() const override { return Type::CAMERA; };

	void MoveRight(float dt);
	void MoveForward(float dt);
	void Rotate(float dx, float dy);

	void Update();
};