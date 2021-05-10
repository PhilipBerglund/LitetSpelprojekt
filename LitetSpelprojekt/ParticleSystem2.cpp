#include "ParticleSystem2.h"
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

ParticleSystem2::ParticleSystem2(UINT maxParticles, float minVelocity, float maxVelocity)
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
		const float x = Random::Real(-100.0f, 100.0f);
		const float z = Random::Real(-100.0f, 100.0f);
		//Put particle above ground
		const float y = Random::Real(0.0f, 150.0f);

		particles[i].position = XMFLOAT4(x, y, z, 1);
		particles[i].size = XMFLOAT2(2.0f, 2.0f);
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

ParticleSystem2::~ParticleSystem2()
{
}

void ParticleSystem2::SetEyePos(const XMFLOAT3& eyePos)
{
	this->eyePos = eyePos;
}

void ParticleSystem2::SetEmitPos(const XMFLOAT3& emitPos)
{
	this->emitPos = emitPos;
}

void ParticleSystem2::SetEmitDir(const XMFLOAT3& emitDir)
{
	this->emitDir = emitDir;
}

void ParticleSystem2::Reset()
{
	this->isFirstRun = true;
	this->age = 0.0f;
}

void ParticleSystem2::Update(float dt)
{
	for (int i = 0; i < maxParticles; i++)
	{
		particles[i].position.y -= dt * velocity;
		if (particles[i].position.y <= 0)
		{
			particles[i].position.y = 200;
		}
	}
	UpdateBuffer(particles);
	
}

bool ParticleSystem2::UpdateBuffer(Particle* particles)
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

bool ParticleSystem2::Draw()
{
	//Particle* particle;
	//particles = new Particle[maxParticles];

	//for (int i = 0; i < maxParticles; i++)
	//{
	//	//Random position between -1 and 1
	//	const float x = Random::Real(-100.0f, 100.0f);
	//	const float z = Random::Real(-10.0f, 10.0f);
	//	//Put particle above ground somehow??
	//	const float y = 20;/*(0.3f * (0.1f * z * sinf(0.1f * x) + 0.1f * x * cosf(0.1f * z))) + 5;*/

	//	particles[i].position = XMFLOAT4(x, y, z, 1);
	//	particles[i].size = XMFLOAT2(2.0f, 2.0f);
	//}

	//D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	//if (FAILED(Graphics::GetDeviceContext().Map(GSParticleVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	//{
	//	return false;
	//}
		
	//particle = (Particle*)mappedResource.pData;
	//memcpy(particle, (void*)particles, (sizeof(Particle) * maxParticles));
	//Graphics::GetDeviceContext().Unmap(GSParticleVB.Get(), 0);

	return true;
}
