#include "SmokeSystem.h"

SmokeSystem::SmokeSystem(UINT maxParticles, float minVelocity, float maxVelocity, XMFLOAT4 startPosition, float maxParticleRange)	
	:origin(startPosition), maxParticleRange(maxParticleRange)
{
	this->velocity = Random::Real(minVelocity, maxVelocity);
	this->maxParticles = maxParticles;
	particles = new Particle[maxParticles];

	if (!particles)
		return;

	for (int i = 0; i < maxParticles; i++)
	{
		float x, z;
		this->angle = Random::Real(-0.7f, 0.7f);
		this->length = Random::Real(-0.7f, 0.7f);
		x = angle * length;
		this->angle = Random::Real(-0.7f, 0.7f);
		this->length = Random::Real(-0.7f, 0.7f);
		z = angle * length;
		XMVECTOR dir = { x, 1, z };
		XMVector3Normalize(dir);
		XMStoreFloat3(&particles[i].direction, dir);

		const float startX = 5.0f;
		const float startY = Random::Real(origin.y,maxParticleRange);
		const float startZ = 5.0f;

		particles[i].position = { startPosition.x, startY, startPosition.z, 1.0f };
		particles[i].size = XMFLOAT2(0.8, 0.8);
	}

	D3D11_BUFFER_DESC vBDesc = {};
	vBDesc.Usage = D3D11_USAGE_DYNAMIC;
	vBDesc.ByteWidth = sizeof(Particle) * (maxParticles);
	vBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vBDesc.MiscFlags = 0;
	vBDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = particles;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT hr = Graphics::GetDevice().CreateBuffer(&vBDesc, &vertexData, &GSParticleVB);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	UpdateBuffer(particles);
}

void SmokeSystem::SetEyePos(const XMFLOAT3& eyePos)
{
	this->eyePos = eyePos;
}

void SmokeSystem::SetEmitPos(const XMFLOAT3& emitPos)
{
	this->emitPos = emitPos;
}

void SmokeSystem::SetEmitDir(const XMFLOAT3& emitDir)
{
	this->emitDir = emitDir;
}

void SmokeSystem::Reset()
{
	this->isFirstRun = true;
	//this->age = 0.0f;
}

void SmokeSystem::Update(float dt)
{
	for (int i = 0; i < maxParticles; i++)
	{
		
		particles[i].position.x += dt * velocity * particles[i].direction.x;
		particles[i].position.y += dt * velocity * particles[i].direction.y;
		particles[i].position.z += dt * velocity * particles[i].direction.z;
		if (particles[i].position.y >= maxParticleRange)
		{
			particles[i].position = origin;
			
		}
	}
	UpdateBuffer(particles);
}

bool SmokeSystem::UpdateBuffer(Particle* particle)
{
	//Particle* particle;

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	if (FAILED(Graphics::GetDeviceContext().Map(GSParticleVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		Error("FAILED TO UPDATE PARTICLE BUFFER");
		return false;
	}

	particle = (Particle*)mappedResource.pData;
	memcpy(particle, (void*)particles, (sizeof(Particle) * maxParticles));
	Graphics::GetDeviceContext().Unmap(GSParticleVB.Get(), 0);

	return true;
}
