#include "ParticleShader2.h"
#include "Scene.h"

void ParticleShader2::SetShader(ShaderData&data)
{
	Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Graphics::GetDeviceContext().IASetInputLayout(data.particle2Layout.Get());
	Graphics::GetDeviceContext().VSSetShader(data.particle2VS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetShader(data.particle2PS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().GSSetShader(data.particle2GS.Get(), nullptr, 0);
}

void ParticleShader2::Render(ShaderData& data, Scene& scene)
{
	SetShader(data);

	unsigned int stride = sizeof(ParticleSystem2::Particle);
	unsigned int offset = 0;

	const auto& particleSystem2 = scene.GetParticleSystems2();

	for (const auto& particleSystem2 : particleSystem2)
	{
		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, particleSystem2->GSParticleVB.GetAddressOf() , &stride, &offset);
		Graphics::GetDeviceContext().PSSetShaderResources(0, 1, data.particle2TexSRV.GetAddressOf());
		Graphics::GetDeviceContext().Draw(particleSystem2->maxParticles, 0);
	}
	Graphics::GetDeviceContext().GSSetShader(nullptr, nullptr, 0);
}