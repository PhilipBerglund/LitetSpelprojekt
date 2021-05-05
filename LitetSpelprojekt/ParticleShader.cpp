#include "ParticleShader.h"

#include "Scene.h"

void ParticleShader::SetShader(ShaderData& data)
{
	Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Graphics::GetDeviceContext().IASetInputLayout(data.particleLayout.Get());
	Graphics::GetDeviceContext().VSSetShader(data.particleVertexShader.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetShader(data.particlePixelShader.Get(), nullptr, 0);
}

void ParticleShader::Render(ShaderData& data, Scene& scene)
{
	SetShader(data);

	unsigned int stride = sizeof(ParticleSystem::ParticleVertex);
	unsigned int offset = 0;

	const auto& particleSystems = scene.GetParticleSystems();

	for (const auto& particleSystem : particleSystems)
	{
		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, particleSystem->vertexBuffer.GetAddressOf(), &stride, &offset);
		Graphics::GetDeviceContext().Draw(particleSystem->vertexCount, 0);
	}
}
