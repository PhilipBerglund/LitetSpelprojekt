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
		XMFLOAT3 initPos;
		XMFLOAT3 initVel;
		XMFLOAT2 size;
		float particleAge;
		unsigned int type;
	};

	UINT maxParticles;
	bool isFirstRun;
	float gameTime;
	float timeStep;
	float age;

	XMFLOAT3 eyePos;
	XMFLOAT3 emitPos;
	XMFLOAT3 emitDir;
public:
	ParticleSystem2();
	ParticleSystem2(UINT maxParticles, bool isFirstRun, float gameTime, float timeStep, float age, XMFLOAT3 eyePos, XMFLOAT3 emitPos, XMFLOAT3 emitDir);
	~ParticleSystem2();

	//bool CreateRandomTexture(ShaderData& shaderData);
	float GetAge()const { return age; };
	void SetEyePos(const XMFLOAT3& eyePos);
	void SetEmitPos(const XMFLOAT3& emitPos);
	void SetEmitDir(const XMFLOAT3& emitDir);
	void Initialize();
	void Reset();
	void Update(float dt, float gameTime);
	void Draw(const Camera& cam);
};