#pragma once
#include "GameObject.h"

class Camera :GameObject
{
private:
	XMMATRIX perspectiveMatrix;
	XMMATRIX viewMatrix;
	XMFLOAT3 forward;

	float pitch;
	float yaw;
	float rotationSpeed;
	float speed;
public:
	Camera();
	Camera(float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetPerspectiveMatrix() const;

	void Rotate(float dx, float dy);

	// Inherited via GameObject
	virtual void Update() override;
};