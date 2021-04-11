#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <string>

#include "GameObject.h"
#include "Geometry.h"

enum class EmissionType { CONE, CUBE };

class ParticleSystem
{
private:
	struct Particle
	{
		XMFLOAT3 position;
		XMFLOAT3 color;
		float velocity;
		bool active;
	};

	XMFLOAT3 bounds;
	XMFLOAT3 center;

	float baseVelocity;
	float velocityVariation;
};