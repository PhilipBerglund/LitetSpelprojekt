#pragma once
#include "GameObject.h"
#include "Drawable.h"

struct LightAttributes
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

class Light :public GameObject, Drawable
{
private:
	LightAttributes attributes;
	XMMATRIX viewMatrix;
	XMMATRIX perspectiveMatrix;
	XMFLOAT3 forward;
public:
	Light();
	Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, float FOV, float aspectRatio, float nearZ, float farZ, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetPerspectiveMatrix() const;

	Type type() const override { return Type::LIGHT; };

	LightAttributes GetAttributes() const;

	// Inherited via GameObject
	virtual void Update() override;

	// Inherited via Drawable
	virtual void UpdateBuffers(ID3D11DeviceContext* context) override;
};