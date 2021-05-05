#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "Graphics.h"
#include <vector>

using namespace DirectX;

class ParticleSystem
{
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

    XMFLOAT3 deviation;
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

    //ComPtr<ID3D11PixelShader> particlePS;
    //ComPtr<ID3D11VertexShader> particleVS;



    /*bool LoadTexture(ID3D11Device* device, std::string textureFile);
    void ReleaseTexture();*/

    bool InitializeSystem();

    bool InitializeBuffers();

    void EmitParticles(float dt);
    void UpdateParticles(float frameTime);
    void KillParticles();

    bool UpdateBuffer(XMFLOAT3 cameraPosition);
    void RenderBuffers();

    bool CreateRandomTexture();

public:
    ParticleSystem();
    ParticleSystem(XMFLOAT3 bounds, XMFLOAT3 center, float velocity, float velocityVariation, int particlesPerSecond, int maxParticles, float size);
    ~ParticleSystem() = default;
    bool Initialize();
    ////bool Initialize(ID3D11Device* device, std::string textureFile);
    bool Frame(float frameTime, XMFLOAT3 cameraPosition);
    void Render();
    //ID3D11ShaderResourceView* GetTexture();
    int GetVertexCount() const;
    void Update(float dt, XMFLOAT3 cameraPosition);
};
