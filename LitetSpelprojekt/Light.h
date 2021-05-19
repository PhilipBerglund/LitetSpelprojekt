#pragma once
#include "GameObject.h"

struct LightAttributes
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT3 direction;
	XMFLOAT4 viewPosition;
};

class Light :public GameObject
{
private:
	LightAttributes attributes;
	XMMATRIX viewMatrix;
	XMMATRIX perspectiveMatrix;
	XMMATRIX orthographicMatrix;
	XMFLOAT3 forward;
public:
	Light();
	Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	XMMATRIX GetViewMatrix() const			{ return this->viewMatrix; };
	XMMATRIX GetPerspectiveMatrix() const	{ return this->perspectiveMatrix; };
	XMMATRIX GetOrthographicMatrix() const	{ return this->orthographicMatrix; };
	LightAttributes GetAttributes() const	{ return this->attributes; };

	Type type() const override { return Type::LIGHT; };
};