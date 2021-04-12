#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <string>

#include "GameObject.h"
#include "Geometry.h"

enum class EmissionType { CONE, CUBE };

class ParticleSystem : public GameObject
{
private:
	struct Particle
	{
		XMFLOAT3 direction;
		XMFLOAT3 position;
		XMFLOAT3 color;
		float velocity;
		bool active;
	};

	float radius;
	XMFLOAT3 bounds;
	float coneAngel;

	float baseVelocity;
	float velocityVariation;

	float size;
	int maxParticles;
	int particlesPerSecond;
	float timeSinceLastSpawn = 0;

	int vertexCount = 0;
	Vertex* vertices;
	ComPtr<ID3D11Buffer> vertexBuffer;
	std::vector<Particle> activeParticles;
	EmissionType type;

	void EmitParticles(float dt);
	void UpdateParticles(float dt);
	void KillParticles();
public:
	ParticleSystem();
	ParticleSystem(XMFLOAT3 bounds, float velocity, float velocityVariation, float size, int maxParticles, int particlesPerSecond, EmissionType type, XMFLOAT3 position, XMFLOAT3 rotation = { 0,0,0 }, float coneAngel = 0, XMFLOAT3 scale = { 1,1,1 });
	bool Initialize(ID3D11Device& device);
	bool Update(float dt, XMFLOAT3 cameraPosition);
	Vertex& GetVertices() const;
	int GetVertexCount() const;
};