#include "ShadowMapShader.h"
#include "Scene.h"

void ShadowMapShader::SetShadowMapShader(ShaderData& data)
{
	Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Graphics::GetDeviceContext().IASetInputLayout(data.rainLayout.Get());
	Graphics::GetDeviceContext().VSSetShader(data.shadowMapVS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetShader(nullptr, nullptr, 0);
}

void ShadowMapShader::RenderShadowMap(ShaderData& data, Scene& scene)
{
	SetShadowMapShader(data);

	shadowMap.UpdateLightAndShadow(scene.GetLights());

	

}
