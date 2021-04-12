#include "Shader.h"
#include "ShaderLoader.h"
#include <iostream>

bool Shader::UpdateBuffers(ID3D11DeviceContext& context, const Model& model, Light light, XMMATRIX viewMatrix,
							XMMATRIX perspectiveMatrix, XMFLOAT3 cameraPosition)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	//VERTEX SHADER BUFFER(S)
	if (FAILED(context.Map(VS_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	VS vertexShaderData = {};
	XMStoreFloat4x4(&vertexShaderData.worldMatrix, XMMatrixTranspose(model.GetMatrix()));

	XMMATRIX WVP = model.GetMatrix() * viewMatrix * perspectiveMatrix;
	XMStoreFloat4x4(&vertexShaderData.viewMatrix, XMMatrixTranspose(WVP));

	XMStoreFloat4x4(&vertexShaderData.perspectiveMatrix, XMMatrixTranspose(perspectiveMatrix));
	XMStoreFloat4x4(&vertexShaderData.lightViewMatrix, XMMatrixTranspose(light.GetViewMatrix()));
	XMStoreFloat4x4(&vertexShaderData.lightPerspectiveMatrix, XMMatrixTranspose(light.GetPerspectiveMatrix()));

	memcpy(mappedResource.pData, &vertexShaderData, sizeof(VS));
	context.Unmap(VS_Buffer.Get(), 0);

	//PIXEL SHADER BUFFER(S)
	PS pixelShaderData = {};
	if (FAILED(context.Map(PS_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	memcpy(mappedResource.pData, &cameraPosition, sizeof(PS));
	context.Unmap(PS_Buffer.Get(), 0);

	return true;
}

bool Shader::Initialize(ID3D11Device& device, HWND window)
{
	std::string byteCode;
	if (!LoadVertexShader(device, vertexShader, vs_path, byteCode))
		return false;

	if (!LoadPixelShader(device, pixelShader, ps_path))
		return false;

	//INPUT LAYOUT
	const UINT numElements = 3;
	D3D11_INPUT_ELEMENT_DESC inputDesc[numElements] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(device.CreateInputLayout(inputDesc, numElements, byteCode.c_str(), byteCode.length(), &layout)))
		return false;

	//BUFFERS
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VS);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (FAILED(device.CreateBuffer(&bufferDesc, nullptr, &VS_Buffer)))
		return false;

	bufferDesc.ByteWidth = sizeof(PS);
	if (FAILED(device.CreateBuffer(&bufferDesc, nullptr, &PS_Buffer)))
		return false;

	return true;
}

void Shader::SetShader(ID3D11DeviceContext& context)
{
	context.IASetInputLayout(layout.Get());
	context.VSSetShader(vertexShader.Get(), nullptr, 0);
	context.PSSetShader(pixelShader.Get(), nullptr, 0);
}

void Shader::Render(ID3D11DeviceContext& context, const Model& model, Light light, XMMATRIX viewMatrix, XMMATRIX perspectiveMatrix, XMFLOAT3 cameraPosition)
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	auto buffer = &model.GetVertexBuffer();
	context.IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

	UpdateBuffers(context, model, light, viewMatrix, perspectiveMatrix, cameraPosition);

	context.PSSetConstantBuffers(0, 1, PS_Buffer.GetAddressOf());
	context.VSSetConstantBuffers(0, 1, VS_Buffer.GetAddressOf());

	context.Draw(model.GetVertexCount(), 0);
}