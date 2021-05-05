#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "Graphics.h"
#include <vector>

using namespace DirectX;

enum class EmitterType
{
    NONE,
    CUBE,
    CONE
};

class ParticleSystem
{
    
    
    friend class ParticleShader;
private:

    EmitterType emitterType;

    struct Particle
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
        float velocity;
        XMFLOAT3 direction;
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

    ID3D11Texture1D* randomTex;
    ID3D11ShaderResourceView* randomTexSRV;

    int vertexCount;

    ParticleVertex* vertices;
    ComPtr<ID3D11Buffer> vertexBuffer;
    std::vector<Particle> activeParticles;

    void EmitParticles(float dt);

    void KillParticles();

    bool UpdateBuffer(XMFLOAT3 cameraPosition);

    bool CreateRandomTexture();
public:

    ParticleSystem();
    ParticleSystem(EmitterType emitterType, XMFLOAT3 bounds, XMFLOAT3 center, float velocity, float velocityVariation, int particlesPerSecond, int maxParticles, float size);
    void Update(float dt, XMFLOAT3 cameraPosition);
};
