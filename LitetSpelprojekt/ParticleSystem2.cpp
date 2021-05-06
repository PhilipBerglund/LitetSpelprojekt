#include "ParticleSystem2.h"

ParticleSystem2::ParticleSystem2()
{
	this->maxParticles = 0;
	this->isFirstRun = true;
	this->gameTime = 0;
	this->timeStep = 0;
	this->age = 0;
	this->eyePos = { 0,0,0 };
	this->emitPos = { 0,0,0 };
	this-> emitDir = { 0,0,0 };
}

ParticleSystem2::ParticleSystem2(UINT maxParticles, bool isFirstRun, float gameTime, float timeStep, float age, XMFLOAT3 eyePos, XMFLOAT3 emitPos, XMFLOAT3 emitDir)
{
}

ParticleSystem2::~ParticleSystem2()
{
}

//CreateRandomTexture
/*
bool ParticleSystem2::CreateRandomTexture(ShaderData& shaderData)
{
	XMFLOAT4 randomValues[1024];

	for (int i = 0; i < 1024; i++)
	{
		randomValues[i].x = Random::Real(-1.0f, 1.0f);
		randomValues[i].y = Random::Real(-1.0f, 1.0f);
		randomValues[i].z = Random::Real(-1.0f, 1.0f);
		randomValues[i].w = Random::Real(-1.0f, 1.0f);
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

	Graphics::GetDevice().CreateTexture1D(&texDesc, &data, &shaderData.particle2RandomTexture);

	//Creating SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc;
	SRVdesc.Format = texDesc.Format;
	SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	SRVdesc.Texture1D.MipLevels = texDesc.MipLevels;
	SRVdesc.Texture1D.MostDetailedMip = 0;

	HRESULT hr = Graphics::GetDevice().CreateShaderResourceView(shaderData.particle2RandomTexture.Get(), &SRVdesc, &shaderData.particle2RandomTexSRV);
	if (FAILED(hr))
	{
		Error("FAILED TO CREATE RANDOM TEXTURE!");
	}

    return true;
}
*/
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

void ParticleSystem2::Initialize()
{

}

void ParticleSystem2::Reset()
{
	this->isFirstRun = true;
	this->age = 0.0f;
}

void ParticleSystem2::Update(float dt, float gameTime)
{
}

void ParticleSystem2::Draw(const Camera& cam)
{
	XMMATRIX view = cam.GetViewMatrix();
	XMMATRIX proj = cam.GetPerspectiveMatrix();
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
}
