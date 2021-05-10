#include "ParticleShader.h"
#include "Scene.h"

void ParticleShader::SetRainShader(ShaderData&data)
{
	//----Rain----
	Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Graphics::GetDeviceContext().IASetInputLayout(data.rainLayout.Get());
	Graphics::GetDeviceContext().VSSetShader(data.rainVS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetShader(data.rainPS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().GSSetShader(data.rainGS.Get(), nullptr, 0);
}

void ParticleShader::SetSmokeShader(ShaderData& data)
{
	//----Smoke----
	//Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Graphics::GetDeviceContext().IASetInputLayout(data.smokeLayout.Get());
	Graphics::GetDeviceContext().VSSetShader(data.smokeVS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetShader(data.smokePS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().GSSetShader(data.smokeGS.Get(), nullptr, 0);
}

void ParticleShader::RenderRain(ShaderData& data, Scene& scene)
{
	SetRainShader(data);

	unsigned int stride = sizeof(RainSystem::Particle);
	unsigned int offset = 0;

	const auto& rainSystem = scene.GetRainSystem();

	for (const auto& rainSystem : rainSystem)
	{
		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, rainSystem->GSParticleVB.GetAddressOf() , &stride, &offset);
		Graphics::GetDeviceContext().PSSetShaderResources(0, 1, data.rainTexSRV.GetAddressOf());
		Graphics::GetDeviceContext().Draw(rainSystem->maxParticles, 0);
	}
	Graphics::GetDeviceContext().GSSetShader(nullptr, nullptr, 0);
}

void ParticleShader::RenderSmoke(ShaderData& data, Scene& scene)
{
	SetSmokeShader(data);

	unsigned int stride = sizeof(SmokeSystem::Particle);
	unsigned int offset = 0;

	const auto& smokeSystem = scene.GetSmokeSystem();

	for (const auto& smokeSystem : smokeSystem)
	{
		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, smokeSystem->GSParticleVB.GetAddressOf(), &stride, &offset);
		Graphics::GetDeviceContext().PSSetShaderResources(0, 1, data.smokeTexSRV.GetAddressOf());
		Graphics::GetDeviceContext().Draw(smokeSystem->maxParticles, 0);
	}
	Graphics::GetDeviceContext().GSSetShader(nullptr, nullptr, 0);
}
