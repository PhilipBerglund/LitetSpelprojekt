#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Random.h"
#include "Graphics.h"
#include "Camera.h"
#include "ShaderData.h"

using namespace DirectX;

class SmokeSystem
{
	friend class ParticleShader;
private:
	struct Particle
	{
		XMFLOAT4 position;
		XMFLOAT2 size;
		XMFLOAT3 direction;
		float age;
	};

	UINT maxParticles;
	float velocity;
	bool isFirstRun;
	float gameTime;
	float timeStep;

	float maxParticleRange;
	float angle;
	float length;
	XMFLOAT4 origin;

	Particle* particles;
	ComPtr<ID3D11Buffer> GSParticleVB;
	XMFLOAT3 eyePos;
	XMFLOAT3 emitPos;
	XMFLOAT3 emitDir;
public:
	SmokeSystem() = default;
	SmokeSystem(UINT maxParticles, float minVelocity, float maxVelocity, XMFLOAT4 startPosition, float maxParticleRange);

	//float GetAge()const { return age; }
	void SetEyePos(const XMFLOAT3& eyePos);
	void SetEmitPos(const XMFLOAT3& emitPos);
	void SetEmitDir(const XMFLOAT3& emitDir);
	void Reset();
	void Update(float dt);
	bool UpdateBuffer(Particle* particle);
};