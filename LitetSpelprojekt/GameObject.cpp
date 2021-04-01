#include "GameObject.h"

GameObject::GameObject(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	transform.position = position;
	transform.rotation = rotation;
	transform.scale = scale;
}

void GameObject::SetScale(XMFLOAT3 scale)
{
	this->transform.scale = scale;
}

void GameObject::SetRotation(XMFLOAT3 rotation)
{
	this->transform.rotation = rotation;
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	this->transform.position = position;
}

XMFLOAT3 GameObject::GetScale() const
{
	return this->transform.scale;
}

XMFLOAT3 GameObject::GetRotation() const
{
	return this->transform.rotation;
}

XMFLOAT3 GameObject::GetPosition() const
{
	return this->transform.position;
}
