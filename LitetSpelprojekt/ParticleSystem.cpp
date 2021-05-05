#include "ParticleSystem.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "Random.h"

ParticleSystem::ParticleSystem()
{
	//this->texture = nullptr;
	this->vertices = nullptr;
	this->vertexBuffer = nullptr;
	this->vertexCount = 0;
	this->velocity = 0;
	this->velocityVariation = 0;
	this->size = 0;
	this->particlesPerSecond = 0;
	this->maxParticles = 0;
	this->deviation = { 0, 0, 0 };
	this->timeSinceLastSpawn = 0;
	this->bounds = { 0,0,0 };
	this->center = { 0,0,0 };
}

//bool ParticleSystem::Initialize()
//{
//	if (!InitializeSystem())
//		return false;

//	if (!InitializeBuffers())
//		return false;

//	return true;
//}

ParticleSystem::ParticleSystem(XMFLOAT3 bounds, XMFLOAT3 center, float velocity, float velocityVariation, int particlesPerSecond, int maxParticles, float size)
	:bounds({ 10.0f, 10.0f, 10.0f }), center({ 0.0f,10.0f,0.0f }), velocity(1.0f), velocityVariation(0.9f), particlesPerSecond(50), maxParticles(500), size(0.006f)
{
	vertexCount = maxParticles * 6; //TWO TRIANGLES
	vertices = new ParticleVertex[vertexCount];
	if (!vertices)
		return;

	//VERTEX BUFFER
	D3D11_BUFFER_DESC vBufferDesc = {};
	vBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vBufferDesc.ByteWidth = sizeof(ParticleVertex) * vertexCount;
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vBufferDesc.MiscFlags = 0;
	vBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	HRESULT hr = Graphics::GetDevice().CreateBuffer(&vBufferDesc, &vertexData, &vertexBuffer);
	if FAILED(hr)
	{
		Print("FAILED TO CREATE BUFFER");
		return;
	}
}

void ParticleSystem::Update(float dt, XMFLOAT3 cameraPosition)
{
	EmitParticles(dt);

	for (int i = 0; i < activeParticles.size(); ++i)
	{
		activeParticles[i].position.y -= (activeParticles[i].velocity * dt * 0.001f);
	}

	if (!UpdateBuffer(cameraPosition))
	{
		Print("FAILED TO UPDATE PARTICLE SYSTEM BUFFER");
		return;
	}
}

void ParticleSystem::Render()
{
	RenderBuffers();
}

int ParticleSystem::GetVertexCount() const
{
	return activeParticles.size() * 6;
}

void ParticleSystem::EmitParticles(float dt)
{
	bool emitNewParticle = false;
	bool found = false;

	XMFLOAT3 spawnPosition;
	float particleVelocity;
	int index = 0;

	timeSinceLastSpawn += dt;

	if (timeSinceLastSpawn > (1000.0f / particlesPerSecond))
	{
		timeSinceLastSpawn = 0.0f;
		emitNewParticle = true;
	}

	if ((emitNewParticle == true) && (activeParticles.size() < (maxParticles - 1.0)))
	{
		spawnPosition.x = center.x + Random::Real(-bounds.x, bounds.x);
		spawnPosition.y = center.y;
		spawnPosition.z = center.z + Random::Real(-bounds.z, bounds.z);

		particleVelocity = velocity + Random::Real() * velocityVariation;

		Particle newParticle = {};
		newParticle.position = spawnPosition;
		newParticle.velocity = particleVelocity;

		activeParticles.push_back(newParticle);
	}
}

//void ParticleSystem::KillParticles()
//{
//	auto removeFrom = std::remove_if(activeParticles.begin(), activeParticles.end(), [this](const Particle& p) 
//	{
//		return p.position.y < (center.y - bounds.y);
//	});

//	activeParticles.erase(removeFrom, activeParticles.end());
//}

bool ParticleSystem::UpdateBuffer(XMFLOAT3 cameraPosition)
{
	int index = 0;

	ParticleVertex* vertex;
	XMMATRIX worldMatrix;
	XMMATRIX translationMatrix;
	XMMATRIX rotationMatrix;
	float angle;

	for (int i = 0; i < activeParticles.size(); ++i)
	{
		//BOTTOM LEFT
		vertices[index].position = XMFLOAT3(-size, -size, size);
		vertices[index].color = XMFLOAT4(activeParticles[i].color.x, activeParticles[i].color.y, activeParticles[i].color.z, 1.0f);
		index++;

		//TOP LEFT
		vertices[index].position = XMFLOAT3(-size, size, size);
		vertices[index].color = XMFLOAT4(activeParticles[i].color.x, activeParticles[i].color.y, activeParticles[i].color.z, 1.0f);
		index++;

		//BOTTOM RIGHT
		vertices[index].position = XMFLOAT3(size, -size, size);
		vertices[index].color = XMFLOAT4(activeParticles[i].color.x, activeParticles[i].color.y, activeParticles[i].color.z, 1.0f);
		index++;

		//BOTTOM RIGHT
		vertices[index].position = XMFLOAT3(size, -size, size);
		vertices[index].color = XMFLOAT4(activeParticles[i].color.x, activeParticles[i].color.y, activeParticles[i].color.z, 1.0f);
		index++;

		//TOP LEFT
		vertices[index].position = XMFLOAT3(-size, size, size);
		vertices[index].color = XMFLOAT4(activeParticles[i].color.x, activeParticles[i].color.y, activeParticles[i].color.z, 1.0f);
		index++;

		//TOP RIGHT
		vertices[index].position = XMFLOAT3(size, size, size);
		vertices[index].color = XMFLOAT4(activeParticles[i].color.x, activeParticles[i].color.y, activeParticles[i].color.z, 1.0f);
		index++;

		for (int j = index - 6; j < index; ++j)
		{
			angle = atan2(activeParticles[i].position.x - cameraPosition.x, activeParticles[i].position.z - cameraPosition.z);
			rotationMatrix = XMMatrixRotationY(angle);
			translationMatrix = XMMatrixTranslation(activeParticles[i].position.x, activeParticles[i].position.y, activeParticles[i].position.z);
			worldMatrix = XMMatrixMultiply(rotationMatrix, translationMatrix);
			XMStoreFloat3(&vertices[j].position, XMVector3Transform(XMLoadFloat3(&vertices[j].position), worldMatrix));
		}
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	if FAILED(Graphics::GetDeviceContext().Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))
		return false;

	vertex = (ParticleVertex*)mappedResource.pData;
	memcpy(vertex, (void*)vertices, (sizeof(ParticleVertex) * vertexCount));
	Graphics::GetDeviceContext().Unmap(vertexBuffer.Get(), 0);

	return true;
}

void ParticleSystem::RenderBuffers()
{
	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;

	Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ParticleSystem::CreateRandomTexture()
{
	XMFLOAT4 randomValues[1024];

	for(int i = 0; i < 1024; i++)
	{
		randomValues[i].x = Random::Real(-1.0f,1.0f);
		randomValues[i].y = Random::Real(-1.0f,1.0f);
		randomValues[i].z = Random::Real(-1.0f,1.0f);
		randomValues[i].w = Random::Real(-1.0f,1.0f);
	}

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = randomValues;
	data.SysMemPitch = 1024 * sizeof(XMFLOAT4);
	data.SysMemSlicePitch = 0;

	//Creating Texture
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	randomTex = 0;
	Graphics::GetDevice().CreateTexture1D(&texDesc, &data, &randomTex);

	//Creating SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc;
	SRVdesc.Format = texDesc.Format;
	SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	SRVdesc.Texture1D.MipLevels = texDesc.MipLevels;
	SRVdesc.Texture1D.MostDetailedMip = 0;

	randomTexSRV = 0;
	Graphics::GetDevice().CreateShaderResourceView(randomTex, &SRVdesc, &randomTexSRV);
	
	return true;
}