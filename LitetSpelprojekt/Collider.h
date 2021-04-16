#pragma once
#include <math.h>

enum class ColliderType { UNDEFINED, BOX, SPHERE };

struct Collider
{
	ColliderType colliderType = ColliderType::UNDEFINED;
	virtual bool Intersects(Collider& other) = 0;
protected:
	bool BoxBoxCollision(Collider& a, Collider& b);
	bool BoxSphereCollision(Collider& a, Collider& b);
	bool SphereSphereCollision(Collider& a, Collider& b);
};

struct BoxCollider :public Collider
{
	float xMin, xMax, yMin, yMax, zMin, zMax;

	BoxCollider() 
	{ 
		this->colliderType = ColliderType::BOX;
		this->xMin = xMax = yMin = yMax = zMin = zMax = 0;
	}
	BoxCollider(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
		:xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), zMin(zMin), zMax(zMax)
		{ this->colliderType = ColliderType::BOX; }

	// Inherited via Collider
	virtual bool Intersects(Collider& other) override
	{
		bool collided = false;

		switch(other.colliderType)
		{
		case ColliderType::BOX:
			collided = BoxBoxCollision(*this, other);
			break;

		case ColliderType::SPHERE:
			collided = BoxSphereCollision(*this, other);
			break;
		}

		return collided;
	}
};

struct SphereCollider :public Collider
{
	float x, y, z;
	float radius;

	SphereCollider()
	{ 
		this->colliderType = ColliderType::SPHERE; 
		this->x = y = z = radius = 0;
	}

	SphereCollider(float x, float y, float z, float radius)
		:x(x), y(y), z(z), radius(radius)
	{ this->colliderType = ColliderType::SPHERE; }

	virtual bool Intersects(Collider& other) override
	{
		bool collided = false;

		switch (other.colliderType)
		{
		case ColliderType::BOX:
			collided = BoxSphereCollision(other, *this);
			break;

		case ColliderType::SPHERE:
			collided = SphereSphereCollision(other, *this);
			break;
		}

		return collided;
	}
};

inline bool Collider::BoxBoxCollision(Collider& a, Collider& b)
{
	BoxCollider boxA = *dynamic_cast<BoxCollider*>(&a);
	BoxCollider boxB = *dynamic_cast<BoxCollider*>(&b);
	return	(boxA.xMin <= boxB.xMax && boxA.xMax >= boxB.xMin) &&
			(boxA.yMin <= boxB.yMax && boxA.yMax >= boxB.yMin) &&
			(boxA.zMin <= boxB.zMax && boxA.zMax >= boxB.zMin);
}

inline bool Collider::BoxSphereCollision(Collider& box, Collider& sphere)
{
	BoxCollider a = *dynamic_cast<BoxCollider*>(&box);
	SphereCollider b = *dynamic_cast<SphereCollider*>(&sphere);
	float x = fmax(a.xMin, fmin(b.x, a.xMax));
	float y = fmax(a.yMin, fmin(b.y, a.yMax));
	float z = fmax(a.zMin, fmin(b.z, a.zMax));

	float distance = sqrt(	(x - b.x) * (x - b.x) +
							(y - b.y) * (y - b.y) +
							(z - b.z) * (z - b.z));

	return distance < b.radius;
}

inline bool Collider::SphereSphereCollision(Collider& a, Collider& b)
{
	SphereCollider sphereA = *dynamic_cast<SphereCollider*>(&a);
	SphereCollider sphereB = *dynamic_cast<SphereCollider*>(&b);

	float distance = sqrt(	(sphereA.x - sphereB.x) * (sphereA.x - sphereB.x) +
							(sphereA.y - sphereB.y) * (sphereA.y - sphereB.y) +
							(sphereA.z - sphereB.z) * (sphereA.z - sphereB.z));
	return distance < (sphereA.radius + sphereB.radius);
}