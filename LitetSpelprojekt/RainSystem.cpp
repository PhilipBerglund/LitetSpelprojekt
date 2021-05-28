#include "RainSystem.h"
//
//ParticleSystem2::ParticleSystem2()
//{
//	this->maxParticles = 0;
//	this->isFirstRun = true;
//	this->gameTime = 0;
//	this->timeStep = 0;
//	this->age = 0;
//	this->eyePos = { 0,0,0 };
//	this->emitPos = { 0,0,0 };
//	this->emitDir = { 0,0,0 };
//	this->particles = 0;
//	this->velocity = 0;
//}

RainSystem::RainSystem(UINT maxParticles, float minVelocity, float maxVelocity)
{
	this->velocity = Random::Real(minVelocity, maxVelocity);
	this->maxParticles = maxParticles;
	particles = new Particle[maxParticles];
	if (!particles)
	{
		return;
	}

	for (int i = 0; i < maxParticles; i++)
	{
		//Random position between -1 and 1
		const float x = Random::Real(-500.0f, 500.0f);
		const float z = Random::Real(-500.0f, 500.0f);
		//Put particle above ground
		const float y = Random::Real(0.0f, 200.0f);

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

RainSystem::~RainSystem()
{
}

void RainSystem::SetEyePos(const XMFLOAT3& eyePos)
{
	this->eyePos = eyePos;
}

void RainSystem::SetEmitPos(const XMFLOAT3& emitPos)
{
	this->emitPos = emitPos;
}

void RainSystem::SetEmitDir(const XMFLOAT3& emitDir)
{
	this->emitDir = emitDir;
}

void RainSystem::Reset()
{
	this->isFirstRun = true;
	this->age = 0.0f;
}

void RainSystem::Update(const Bounds& bounds, float dt)
{
	for (int i = 0; i < maxParticles; i++)
	{
		particles[i].position.y -= dt * velocity;

		if (particles[i].position.y <= 0)
			particles[i].position.y = 200;

		for (auto& box : bounds.boxes)
			if (box.Contains(XMLoadFloat4(&particles[i].position)) == CONTAINS)
				particles[i].position.y = 200;
	}

	UpdateBuffer(particles);
}

bool RainSystem::UpdateBuffer(Particle* particles)
{
	Particle* particle;

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