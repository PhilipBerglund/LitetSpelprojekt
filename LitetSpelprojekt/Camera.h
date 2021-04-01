#pragma once
#include "GameObject.h"

class Camera :GameObject
{
private:
	XMMATRIX viewMatrix;
public:
	Camera() = default;
	Camera(XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	XMMATRIX GetViewMatrix() const;

	// Inherited via GameObject
	virtual void Update() override;
};