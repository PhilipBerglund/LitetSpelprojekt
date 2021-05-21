#include "RegularShader.h"
#include "Scene.h"

void RegularShader::SetShader(ShaderData& data)
{
	Graphics::GetDeviceContext().IASetInputLayout(data.regularLayout.Get());
	Graphics::GetDeviceContext().VSSetShader(data.regularVertexShader.Get(), nullptr, 0);
	Graphics::GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void RegularShader::UpdatePerMesh(ShaderData& data, Model& model)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};

	//VERTEX SHADER BUFFERS

	//MATRICES
	hr = Graphics::GetDeviceContext().Map(data.regularMatricesBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return;
	}

	ShaderData::MatricesCbuf matrices;
	XMStoreFloat4x4(&matrices.worldMatrix, XMMatrixTranspose(model.GetMatrix()));

	XMMATRIX WVP = model.GetMatrix() * data.viewMatrix * data.projectionMatrix;
	XMStoreFloat4x4(&matrices.WVPMatrix, XMMatrixTranspose(WVP));

	memcpy(mappedResource.pData, &matrices, sizeof(ShaderData::MatricesCbuf));
	Graphics::GetDeviceContext().Unmap(data.regularMatricesBuffer.Get(), 0);
	Graphics::GetDeviceContext().VSSetConstantBuffers(0, 1, data.regularMatricesBuffer.GetAddressOf());

	//PIXEL SHADER BUFFERS

	//MATERIAL
	hr = Graphics::GetDeviceContext().Map(data.regularMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return;
	}

	ShaderData::MaterialCBuf material;
	Material modelMat = model.GetMaterial();

	material.diffuse[0] = modelMat.diffuse[0];
	material.diffuse[1] = modelMat.diffuse[1];
	material.diffuse[2] = modelMat.diffuse[2];

	material.opacity = modelMat.opacity;

	material.ambient[0] = modelMat.ambient[0];
	material.ambient[1] = modelMat.ambient[1];
	material.ambient[2] = modelMat.ambient[2];

	material.shininess = modelMat.shininess;

	material.emissive[0] = modelMat.emissive[0];
	material.emissive[1] = modelMat.emissive[1];
	material.emissive[2] = modelMat.emissive[2];

	material.reflectivity = modelMat.reflectivity;

	material.specular[0] = modelMat.specular[0];
	material.specular[1] = modelMat.specular[1];
	material.specular[2] = modelMat.specular[2];

	memcpy(mappedResource.pData, &material, sizeof(ShaderData::MaterialCBuf));
	Graphics::GetDeviceContext().Unmap(data.regularMaterialBuffer.Get(), 0);
	Graphics::GetDeviceContext().PSSetConstantBuffers(1, 1, data.regularMaterialBuffer.GetAddressOf());

	//JOINTS
	hr = Graphics::GetDeviceContext().Map(data.jointBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
	{
		Error("FAILED TO MAP BUFFER");
		return;
	}

	for (int i = 0; i < model.GetNumJoints(); ++i)
		if (i < MAX_JOINTS)
			data.jointMatrices[i] = model.GetJointTransform(i, model.GetCurrentKeyFrame());

	memcpy(mappedResource.pData, &data.jointMatrices, sizeof(XMFLOAT4X4) * MAX_JOINTS);
	Graphics::GetDeviceContext().Unmap(data.jointBuffer.Get(), 0);
	Graphics::GetDeviceContext().VSSetConstantBuffers(2, 1, data.jointBuffer.GetAddressOf());
}

void RegularShader::Render(ShaderData& data, Scene& scene)
{
	SetShader(data);

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	const auto& models = scene.GetModels();

	for (const auto& pair : models)
	{
		auto& model = pair.second;
		UpdatePerMesh(data, *model);

		Graphics::GetDeviceContext().IASetVertexBuffers(0, 1, model->GetVertexBuffer(), &stride, &offset);

		Graphics::GetDeviceContext().PSSetShader(data.regularTexturePixelShader.Get(), nullptr, 0);
		const float mat[] = { model->GetMaterial().diffuse[0], model->GetMaterial().diffuse[1], model->GetMaterial().diffuse[2] };

		if (model->GetDiffuseTexture())
			Graphics::GetDeviceContext().PSSetShaderResources(0, 1, model->GetDiffuseTexture());

		else if ((mat[0] == 0.5f && mat[1] == 0.5f && mat[2] == 0.5f) ||
			(mat[0] == 1.0f && mat[1] == 1.0f && mat[2] == 1.0f) ||
			(mat[0] == 0.0f && mat[1] == 0.0f && mat[2] == 0.0f))
			Graphics::GetDeviceContext().PSSetShader(data.regularPixelShader.Get(), nullptr, 0);

		else
			Graphics::GetDeviceContext().PSSetShader(data.regularColorPixelShader.Get(), nullptr, 0);

		Graphics::GetDeviceContext().Draw(model->GetVertexCount(), 0);
	}
}