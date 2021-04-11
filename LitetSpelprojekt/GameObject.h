#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class GameObject
{
protected:
	struct Transform
	{
		XMFLOAT3 scale;
		XMFLOAT3 rotation;
		XMFLOAT3 position;
	} transform;

	virtual void Update() = 0;
public:
	GameObject() = default;
	GameObject(XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	void SetScale(XMFLOAT3 scale);
	void SetRotation(XMFLOAT3 rotation);
	void SetPosition(XMFLOAT3 position);

	XMFLOAT3 GetScale() const;
	XMFLOAT3 GetRotation() const;
	XMFLOAT3 GetPosition() const;
};