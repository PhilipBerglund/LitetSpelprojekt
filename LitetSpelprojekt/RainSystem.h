#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Random.h"
#include "Graphics.h"
#include "Camera.h"
#include "ShaderData.h"
#include "Bounds.h"

using namespace DirectX;

class RainSystem
{
	friend class ParticleShader;
private:
	struct Particle
	{
		XMFLOAT4 position;
		XMFLOAT2 size;
	};

	UINT maxParticles;
	float velocity;
	
	bool isFirstRun;
	float gameTime;
	float timeStep;
	float age;

	Particle* particles;
	ComPtr<ID3D11Buffer> GSParticleVB;
	XMFLOAT3 eyePos;
	XMFLOAT3 emitPos;
	XMFLOAT3 emitDir;
public:
	RainSystem() = default;
	RainSystem(UINT maxParticles, float minVelocity, float maxVelocity);
	~RainSystem();

	float GetAge()const { return age; };
	void SetEyePos(const XMFLOAT3& eyePos);
	void SetEmitPos(const XMFLOAT3& emitPos);
	void SetEmitDir(const XMFLOAT3& emitDir);
	void Reset();
	void Update(const Bounds& bounds, float dt);
	bool UpdateBuffer(Particle* particle);
};