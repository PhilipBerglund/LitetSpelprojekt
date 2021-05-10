#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "Graphics.h"
#include <vector>

using namespace DirectX;

class ParticleSystem
{
    friend class ParticleShader;
private:
    struct Particle
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
        float velocity;
    };

    struct ParticleVertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

    XMFLOAT3 bounds;
    XMFLOAT3 center;

    float velocity;
    float velocityVariation;
    float size;
    int particlesPerSecond;
    int maxParticles;

    float timeSinceLastSpawn;

    int vertexCount;

    ParticleVertex* vertices;
    ComPtr<ID3D11Buffer> vertexBuffer;
    std::vector<Particle> activeParticles;

    void EmitParticles(float dt);

    void KillParticles();

    bool UpdateBuffer(XMFLOAT3 cameraPosition);
public:
    ParticleSystem();
    ParticleSystem(XMFLOAT3 bounds, XMFLOAT3 center, float velocity, float velocityVariation, int particlesPerSecond, int maxParticles, float size);
    void Update(float dt, XMFLOAT3 cameraPosition);
};
