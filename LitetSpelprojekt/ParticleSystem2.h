#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Random.h"
#include "Graphics.h"
#include "Camera.h"
#include "ShaderData.h"

using namespace DirectX;

class ParticleSystem2
{
	friend class ParticleShader2;
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
	ParticleSystem2() = default;
	ParticleSystem2(UINT maxParticles, float velocity);
	~ParticleSystem2();

	float GetAge()const { return age; };
	void SetEyePos(const XMFLOAT3& eyePos);
	void SetEmitPos(const XMFLOAT3& emitPos);
	void SetEmitDir(const XMFLOAT3& emitDir);
	void Reset();
	void Update(float dt, float gameTime);
	bool Draw();
};