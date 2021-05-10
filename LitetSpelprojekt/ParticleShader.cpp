#include "ParticleShader.h"
#include "Scene.h"

void ParticleShader::SetShader(ShaderData&data)
{
	Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Graphics::GetDeviceContext().IASetInputLayout(data.rainLayout.Get());
	Graphics::GetDeviceContext().VSSetShader(data.rainVS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetShader(data.rainPS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().GSSetShader(data.rainGS.Get(), nullptr, 0);
}

void ParticleShader::Render(ShaderData& data, Scene& scene)
{
	SetShader(data);

	unsigned int stride = sizeof(RainSystem::Particle);
	unsigned int offset = 0;

	const auto& particleSystem2 = scene.GetRainSystem();

	for (const auto& particleSystem2 : particleSystem2)
	{
		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, particleSystem2->GSParticleVB.GetAddressOf() , &stride, &offset);
		Graphics::GetDeviceContext().PSSetShaderResources(0, 1, data.rainTexSRV.GetAddressOf());
		Graphics::GetDeviceContext().Draw(particleSystem2->maxParticles, 0);
	}
	Graphics::GetDeviceContext().GSSetShader(nullptr, nullptr, 0);
}