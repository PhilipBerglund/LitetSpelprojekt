#pragma once
#include "GameObject.h"

struct LightAttributes
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

class Light :public GameObject
{
private:
	LightAttributes attributes;
	XMMATRIX viewMatrix;
	XMMATRIX perspectiveMatrix;
	XMFLOAT3 forward;
public:
	Light();
	Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	XMMATRIX GetViewMatrix() const			{ return this->viewMatrix; };
	XMMATRIX GetPerspectiveMatrix() const	{ return this->perspectiveMatrix; };
	LightAttributes GetAttributes() const	{ return this->attributes; };

	Type type() const override { return Type::LIGHT; };
};