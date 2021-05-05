#include "ShaderData.h"
#include "Graphics.h"
#include "ShaderLoader.h"

ShaderData::ShaderData()
{
	HRESULT hr;
	std::string byteCode;

	//-----PARTICLE SYSTEM-----
	std::string vs_path = "../x64/Debug/ParticleVS.cso";
	std::string ps_path = "../x64/Debug/ParticlePS.cso";

	//SHADERS
	if (!LoadVertexShader(particleVertexShader, vs_path, byteCode))
		return;

	if (!LoadPixelShader(particlePixelShader, ps_path))
		return;

	//INPUT LAYOUT
	const UINT numElements = 2;
	D3D11_INPUT_ELEMENT_DESC inputDesc[numElements] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	hr = Graphics::GetDevice().CreateInputLayout(inputDesc, numElements, byteCode.c_str(), byteCode.length(), &particleLayout);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE INPUT LAYOUT");
		return;
	}

	//BUFFER
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &viewProjBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}
}

void ShaderData::Update(const Camera& camera)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	//-----GENERAL-----
	cameraPosition = camera.GetPosition();
	viewMatrix = camera.GetViewMatrix();
	projectionMatrix = camera.GetPerspectiveMatrix();

	//-----PARTICLE_SYSTEM-----
	//VIEW PROJ BUFFER
	hr = Graphics::GetDeviceContext().Map(viewProjBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return;
	}

	XMFLOAT4X4 viewProjMatrix;
	XMStoreFloat4x4(&viewProjMatrix, XMMatrixTranspose(projectionMatrix) * XMMatrixTranspose(viewMatrix));

	memcpy(mappedResource.pData, &viewProjMatrix, sizeof(XMFLOAT4X4));
	Graphics::GetDeviceContext().Unmap(viewProjBuffer.Get(), 0);
	
	Graphics::GetDeviceContext().VSSetConstantBuffers(1, 1, viewProjBuffer.GetAddressOf());
}