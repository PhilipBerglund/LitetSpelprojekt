#include "Model.h"

void Model::Update(ID3D11DeviceContext& context)
{
    XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.position));
    XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotation));
    XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));

    worldMatrix = scale * translation * rotation;

    boundingbox.Center = transform.position;
}

Model::Model(const Mesh& mesh)
    :mesh(mesh), name(mesh.name)
{
    transform.position = { mesh.translation[0], mesh.translation[1], mesh.translation[2] };
    transform.rotation = { mesh.rotation[0], mesh.rotation[1], mesh.rotation[2] };
    transform.scale = { mesh.scale[0], mesh.scale[1], mesh.scale[2] };

    XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.position));
    XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotation));
    XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));

    worldMatrix = scale * rotation * translation;

    XMFLOAT4 orientation;
    XMStoreFloat4(&orientation, XMQuaternionIdentity());
    boundingbox = BoundingOrientedBox(transform.position, { 2.0f, 2.0f, 2.0f }, orientation);
}

Model::Model()
    :worldMatrix(XMMatrixIdentity())
{}