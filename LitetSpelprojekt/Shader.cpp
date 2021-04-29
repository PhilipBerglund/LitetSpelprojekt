#include "Shader.h"
#include "ShaderLoader.h"
#include "Scene.h"

bool Shader::UpdateBuffers(const Model& model)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	//VERTEX SHADER BUFFER(S)
	hr = Graphics::GetDeviceContext().Map(VS_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return false;
	}

	XMStoreFloat4x4(&vertexShaderBuffer.worldMatrix, XMMatrixTranspose(model.GetMatrix()));

	XMMATRIX WVP = model.GetMatrix() * XMLoadFloat4x4(&vertexShaderBuffer.viewMatrix) * XMLoadFloat4x4(&vertexShaderBuffer.perspectiveMatrix);

	XMStoreFloat4x4(&vertexShaderBuffer.WVP, XMMatrixTranspose(WVP));
	
	memcpy(mappedResource.pData, &vertexShaderBuffer, sizeof(VS));
	Graphics::GetDeviceContext().Unmap(VS_Buffer.Get(), 0);

	//PIXEL SHADER BUFFER(S)
	PS pixelShaderData = {};
	hr = Graphics::GetDeviceContext().Map(PS_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return false;
	}

	memcpy(mappedResource.pData, &pixelShaderBuffer.cameraPosition, sizeof(PS));
	Graphics::GetDeviceContext().Unmap(PS_Buffer.Get(), 0);

	return true;
}

bool Shader::Initialize(HWND window)
{
	HRESULT hr;
	std::string byteCode;

	if (!LoadVertexShader(vertexShader, vs_path, byteCode))
		return false;

	if (!LoadPixelShader(pixelShader, ps_path))
		return false;

	//INPUT LAYOUT
	const UINT numElements = 7;
	D3D11_INPUT_ELEMENT_DESC inputDesc[numElements] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = Graphics::GetDevice().CreateInputLayout(inputDesc, numElements, byteCode.c_str(), byteCode.length(), &layout);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE INPUT LAYOUT");
		return false;
	}

	//BUFFERS
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VS);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &VS_Buffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return false;
	}

	bufferDesc.ByteWidth = sizeof(PS);

	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &PS_Buffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return false;
	}

	return true;
}

void Shader::SetShader(const Scene& scene)
{
	const auto& camera = scene.GetCamera();
	const auto& lights = scene.GetLights();

	XMStoreFloat4x4(&vertexShaderBuffer.viewMatrix, camera.GetViewMatrix());
	XMStoreFloat4x4(&vertexShaderBuffer.perspectiveMatrix, camera.GetPerspectiveMatrix());
	XMStoreFloat4x4(&vertexShaderBuffer.lightViewMatrix, lights[0]->GetViewMatrix());
	XMStoreFloat4x4(&vertexShaderBuffer.lightPerspectiveMatrix, lights[0]->GetPerspectiveMatrix());

	pixelShaderBuffer.cameraPosition = camera.GetPosition();

	Graphics::GetDeviceContext().IASetInputLayout(layout.Get());
	Graphics::GetDeviceContext().VSSetShader(vertexShader.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetShader(pixelShader.Get(), nullptr, 0);
}

void Shader::Render(const Scene& scene)
{
	SetShader(scene);

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	const auto& models = scene.GetModels();

	for (const auto& model : models)
	{
		UpdateBuffers(*model);

		auto buffer = &model->GetVertexBuffer();
		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

		Graphics::GetDeviceContext().PSSetConstantBuffers(0, 1, PS_Buffer.GetAddressOf());
		Graphics::GetDeviceContext().VSSetConstantBuffers(0, 1, VS_Buffer.GetAddressOf());

		Graphics::GetDeviceContext().Draw(model->GetVertexCount(), 0);
	}
}