#include "SmokeSystem.h"

SmokeSystem::SmokeSystem(UINT maxParticles, float minVelocity, float maxVelocity)
{
	this->velocity = Random::Real(minVelocity, maxVelocity);
	this->maxParticles = maxParticles;
	particles = new Particle[maxParticles];
	if (!particles)
	{
		return;
	}

	this->dirY = 1.0f;

	for (int i = 0; i < maxParticles; i++)
	{
		//dir = (Random::Real(-1.0f, 1.0f), Random::Real(-1.0f, 1.0f), 0.0f);
		
		this->angle = Random::Real(-1.0f, 1.0f);
		this->length = Random::Real(-1.0f, 1.0f);



		const float x = 5.0f;
		const float z = 5.0f;
		const float y = 5.0f;

		particles[i].position = XMFLOAT4(x, y, z, 1);
		particles[i].size = XMFLOAT2(1.0f, 1.0f);
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
		Print("FAILED TO CREATE BUFFER");
		return;
	}

	UpdateBuffer(particles);
}

SmokeSystem::~SmokeSystem()
{
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
	this->age = 0.0f;
}

void SmokeSystem::Update(float dt)
{
	for (int i = 0; i < maxParticles; i++)
	{
		particles[i].position.y += dt * velocity;
		if (particles[i].position.y >= 30.0f)
		{
			particles[i].position.y = 10.0f;
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
