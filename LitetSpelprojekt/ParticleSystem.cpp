#include "ParticleSystem.h"
#include "Random.h"

ParticleSystem::ParticleSystem()
{
	this->bounds = { 10, 10, 10 };
	this->baseVelocity = 1.0f;
	this->velocityVariation = 0.2f;
	this->size = 1.0f;
	this->maxParticles = 50;
	this->particlesPerSecond = 5;
	this->type = EmissionType::CUBE;
	this->vertices = nullptr;
	this->coneAngel = 0;
	this->radius = 0;
}

ParticleSystem::ParticleSystem(XMFLOAT3 bounds, float velocity, float velocityVariation, float size, int maxParticles, int particlesPerSecond, EmissionType type, XMFLOAT3 position, XMFLOAT3 rotation, float coneAngel, XMFLOAT3 scale)
	:GameObject(position, rotation, scale), bounds(bounds), baseVelocity(velocity), velocityVariation(velocityVariation), size(size), maxParticles(maxParticles), particlesPerSecond(particlesPerSecond), type(type), vertices(nullptr), coneAngel(coneAngel)
{
	this->radius = tan(coneAngel) * bounds.y;
}

bool ParticleSystem::Initialize(ID3D11Device& device)
{
	vertexCount = maxParticles * 6;
	vertices = new Vert[vertexCount];
	
	//VERTEX BUFFER
	D3D11_BUFFER_DESC vBufferDesc = {};
	vBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vBufferDesc.ByteWidth = sizeof(Vert) * vertexCount;
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device.CreateBuffer(&vBufferDesc, &vertexData, &vertexBuffer)))
		return false;

	return true;
}

void ParticleSystem::EmitParticles(float dt)
{
	bool emitNewParticle = false;
	bool found = false;

	XMFLOAT3 spawnPosition = {};
	float particleVelocity;
	
	timeSinceLastSpawn += dt;

	if (timeSinceLastSpawn > (1000.0f / particlesPerSecond))
	{
		timeSinceLastSpawn = 0;
		emitNewParticle = true;
	}

	if ((emitNewParticle == true) && (activeParticles.size() < (maxParticles - 1.0)))
	{
		Particle newParticle = {};

		switch (type)
		{
		case EmissionType::CUBE:
			spawnPosition.x = transform.position.x;
			spawnPosition.y = transform.position.y;
			spawnPosition.z = transform.position.z;
			//RANDOM DIRECTION TOWARDS BOUNDS THING
			newParticle.direction = {};
			break;

		case EmissionType::CONE:
			spawnPosition = transform.position;
			XMFLOAT2 center = { spawnPosition.x, spawnPosition.z };

			float x = Random::Real(-radius, radius);
			float z = Random::Real(-radius, radius);

			if (pow(x - center.x, 2) + pow(z - center.x, 2) < pow(radius, 2))
			{
				XMFLOAT3 destination = { x, z, bounds.y };
				newParticle.direction = {
										destination.x - transform.position.x,
										destination.y - transform.position.y,
										destination.z - transform.position.z
										};
			}

			break;
		}

		particleVelocity = baseVelocity * velocityVariation;

		newParticle.position = spawnPosition;
		newParticle.velocity = particleVelocity;
		newParticle.color = { 0.5, 0.5, 0.8 };
		newParticle.active = true;

		//activeParticles.push_back(newParticle);
	}
}

void ParticleSystem::UpdateParticles(float dt)
{
	for (int i = 0; i < activeParticles.size(); ++i)
	{
		float speed = (activeParticles[i].velocity * dt);
		activeParticles[i].position.x -= activeParticles[i].direction.x * speed;
		activeParticles[i].position.y -= activeParticles[i].direction.y * speed;
		activeParticles[i].position.z -= activeParticles[i].direction.z * speed;
	};
}

void ParticleSystem::KillParticles()
{
	auto removeFrom = std::remove_if(activeParticles.begin(), activeParticles.end(), [this](const Particle& p)
		{return p.position.y < (transform.position.y - bounds.y); });
	activeParticles.erase(removeFrom, activeParticles.end());
}