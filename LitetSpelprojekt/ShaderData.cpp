#include "ShaderData.h"
#include "ShaderLoader.h"

ShaderData::ShaderData()
{
	HRESULT hr;
	std::string byteCode;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//SAMPLERS
	Graphics::GetDeviceContext().PSSetSamplers(0, 1, Graphics::GetWrapSampler());

	//-----PARTICLE SYSTEM-----
	//SHADERS
	std::string vs_path = "../x64/Debug/ParticleVS.cso";
	if (!LoadVertexShader(particleVertexShader, vs_path, byteCode))
		return;

	std::string ps_path = "../x64/Debug/ParticlePS.cso";
	if (!LoadPixelShader(particlePixelShader, ps_path))
		return;

	//INPUT LAYOUT
	const unsigned int particleNumElements = 2;
	D3D11_INPUT_ELEMENT_DESC particleInputDesc[particleNumElements] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	hr = Graphics::GetDevice().CreateInputLayout(particleInputDesc, particleNumElements, byteCode.c_str(), byteCode.length(), &particleLayout);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE INPUT LAYOUT");
		return;
	}

	//BUFFER
	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &viewProjBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	//-----REGULAR-----
	//SHADERS
	vs_path = "../x64/Debug/VertexShader.cso";
	if (!LoadVertexShader(regularVertexShader, vs_path, byteCode))
		return;

	ps_path = "../x64/Debug/PixelShader.cso";
	if (!LoadPixelShader(regularPixelShader, ps_path))
		return;

	ps_path = "../x64/Debug/TexturePixelShader.cso";
	if (!LoadPixelShader(regularTexturePixelShader, ps_path))
		return;

	ps_path = "../x64/Debug/ColorPixelShader.cso";
	if (!LoadPixelShader(regularColorPixelShader, ps_path))
		return;

	//INPUT LAYOUT
	const unsigned int regularNumElements = 7;
	D3D11_INPUT_ELEMENT_DESC regularInputDesc[regularNumElements]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = Graphics::GetDevice().CreateInputLayout(regularInputDesc, regularNumElements, byteCode.c_str(), byteCode.length(), &regularLayout);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE INPUT LAYOUT");
		return;
	}

	//BUFFERS
	bufferDesc.ByteWidth = sizeof(MatricesCbuf);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &regularMatricesBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	bufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &regularLightMatrixBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	bufferDesc.ByteWidth = sizeof(MaterialCBuf);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &regularMaterialBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return;
	}

	bufferDesc.ByteWidth = sizeof(CameraCbuf);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &regularCameraBuffer);
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
	Graphics::GetDeviceContext().VSSetConstantBuffers(2, 1, viewProjBuffer.GetAddressOf());

	//-----REGULAR-----
	//CAMERA BUFFER
	hr = Graphics::GetDeviceContext().Map(regularCameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return;
	}

	memcpy(mappedResource.pData, &cameraPosition, sizeof(XMFLOAT3));
	Graphics::GetDeviceContext().Unmap(regularCameraBuffer.Get(), 0);
	Graphics::GetDeviceContext().PSSetConstantBuffers(0, 1, regularCameraBuffer.GetAddressOf());
}