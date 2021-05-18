#include "ShadowMapShader.h"
#include "Scene.h"

void ShadowMapShader::SetShadowMapShader(ShaderData& data)
{
	Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Graphics::GetDeviceContext().IASetInputLayout(data.regularLayout.Get());
	Graphics::GetDeviceContext().VSSetShader(data.shadowMapVS.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetShader(nullptr, nullptr, 0);
}

void ShadowMapShader::UpdatePerMesh(ShaderData& data, Model& model)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	//VERTEX SHADER BUFFERS

	//MATRICES
	hr = Graphics::GetDeviceContext().Map(data.lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return;
	}

	XMMATRIX WVP = model.GetMatrix() * data.lightViewMatrix * data.lightProjectionMatrix;
	XMFLOAT4X4 lightWVP;
	XMStoreFloat4x4(&lightWVP, XMMatrixTranspose(WVP));

	memcpy(mappedResource.pData, &lightWVP, sizeof(XMFLOAT4X4));
	Graphics::GetDeviceContext().Unmap(data.lightBuffer.Get(), 0);
	Graphics::GetDeviceContext().VSSetConstantBuffers(3, 1, data.lightBuffer.GetAddressOf());
}

void ShadowMapShader::RenderShadowMap(ShaderData& data, Scene& scene)
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	SetShadowMapShader(data);

	data.shadowMap.UpdateLightAndShadow(scene.GetLights()[0]->GetRotation(), data.shadowMapMatrix);
	data.shadowMap.BindDsvAndSetNullRenderTarget();

	Graphics::GetDeviceContext().ClearDepthStencilView(data.shadowMap.DepthMapDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	const auto& models = scene.GetModels();

	/*for (const auto& pair : models)
	{
		auto& model = pair.second;
		UpdatePerMesh(data, *model);

		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, model->GetVertexBuffer(), &stride, &offset);

		Graphics::GetDeviceContext().Draw(model->GetVertexCount(), 0);
	}*/
}
