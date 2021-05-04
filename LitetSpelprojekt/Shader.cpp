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

	XMStoreFloat4x4(&vertexCbuf.worldMatrix, XMMatrixTranspose(model.GetMatrix()));

	XMMATRIX WVP = model.GetMatrix() * XMLoadFloat4x4(&vertexCbuf.viewMatrix) * XMLoadFloat4x4(&vertexCbuf.perspectiveMatrix);

	XMStoreFloat4x4(&vertexCbuf.WVP, XMMatrixTranspose(WVP));

	memcpy(mappedResource.pData, &vertexCbuf, sizeof(VS));
	Graphics::GetDeviceContext().Unmap(VS_Buffer.Get(), 0);

	//PIXEL SHADER BUFFER(S)
	PS pixelShaderData = {};
	hr = Graphics::GetDeviceContext().Map(cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return false;
	}

	memcpy(mappedResource.pData, &cameraCbuf.cameraPosition, sizeof(PS));
	Graphics::GetDeviceContext().Unmap(cameraBuffer.Get(), 0);

	hr = Graphics::GetDeviceContext().Map(materialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return false;
	}

	MATERIAL materialData = {};
	Material modelMaterial = model.GetMaterial();

	materialData.diffuse[0] = modelMaterial.diffuse[0];
	materialData.diffuse[1] = modelMaterial.diffuse[1];
	materialData.diffuse[2] = modelMaterial.diffuse[2];

	materialData.opacity = modelMaterial.opacity;

	materialData.ambient[0] = modelMaterial.ambient[0];
	materialData.ambient[1] = modelMaterial.ambient[1];
	materialData.ambient[2] = modelMaterial.ambient[2];

	materialData.shininess = modelMaterial.shininess;

	materialData.emissive[0] = modelMaterial.emissive[0];
	materialData.emissive[1] = modelMaterial.emissive[1];
	materialData.emissive[2] = modelMaterial.emissive[2];

	materialData.reflectivity = modelMaterial.reflectivity;

	materialData.specular[0] = modelMaterial.specular[0];
	materialData.specular[1] = modelMaterial.specular[1];
	materialData.specular[2] = modelMaterial.specular[2];

	memcpy(mappedResource.pData, &materialData, sizeof(MATERIAL));
	Graphics::GetDeviceContext().Unmap(materialBuffer.Get(), 0);

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

	if (!LoadPixelShader(texturePixelShader, texture_ps_path))
		return false;

	if (!LoadPixelShader(colorPixelShader, color_ps_path))
		return false;

	//INPUT LAYOUT
	const UINT numElements = 7;
	D3D11_INPUT_ELEMENT_DESC inputDesc[numElements] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &cameraBuffer);
	if FAILED(hr)
	{
		Error("FAILED TO CREATE BUFFER");
		return false;
	}

	bufferDesc.ByteWidth = sizeof(MATERIAL);
	hr = Graphics::GetDevice().CreateBuffer(&bufferDesc, nullptr, &materialBuffer);
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

	XMStoreFloat4x4(&vertexCbuf.viewMatrix, camera.GetViewMatrix());
	XMStoreFloat4x4(&vertexCbuf.perspectiveMatrix, camera.GetPerspectiveMatrix());
	XMStoreFloat4x4(&vertexCbuf.lightViewMatrix, lights[0]->GetViewMatrix());
	XMStoreFloat4x4(&vertexCbuf.lightPerspectiveMatrix, lights[0]->GetPerspectiveMatrix());

	cameraCbuf.cameraPosition = camera.GetPosition();

	Graphics::GetDeviceContext().IASetInputLayout(layout.Get());
	Graphics::GetDeviceContext().VSSetShader(vertexShader.Get(), nullptr, 0);
	Graphics::GetDeviceContext().PSSetSamplers(0, 1, Graphics::GetWrapSampler());
}

void Shader::Render(const Scene& scene)
{
	SetShader(scene);

	float baseLambert[3] = { 0.5,0.5,0.5 };

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	const auto& models = scene.GetModels();

	for (const auto& model : models)
	{
		UpdateBuffers(*model);

		auto buffer = &model->GetVertexBuffer();
		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

		Graphics::GetDeviceContext().PSSetShader(texturePixelShader.Get(), nullptr, 0);
		Graphics::GetDeviceContext().PSSetConstantBuffers(0, 1, cameraBuffer.GetAddressOf());
		Graphics::GetDeviceContext().PSSetConstantBuffers(1, 1, materialBuffer.GetAddressOf());

		const float mat[] = { model->GetMaterial().diffuse[0], model->GetMaterial().diffuse[1], model->GetMaterial().diffuse[2] };

		if (*model->GetDiffuseTexture())
			Graphics::GetDeviceContext().PSSetShaderResources(0, 1, model->GetDiffuseTexture());

		else if (	(mat[0] == 0.5f && mat[1] == 0.5f && mat[2] == 0.5f) ||
					(mat[0] == 1.0f && mat[1] == 1.0f && mat[2] == 1.0f) ||
					(mat[0] == 0.0f && mat[1] == 0.0f && mat[2] == 0.0f))
			Graphics::GetDeviceContext().PSSetShader(pixelShader.Get(), nullptr, 0);

		else
			Graphics::GetDeviceContext().PSSetShader(colorPixelShader.Get(), nullptr, 0);

		Graphics::GetDeviceContext().VSSetConstantBuffers(0, 1, VS_Buffer.GetAddressOf());

		Graphics::GetDeviceContext().Draw(model->GetVertexCount(), 0);
	}
}