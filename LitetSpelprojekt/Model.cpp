#include "Model.h"
#include "Math.h"

void Model::Update(ID3D11DeviceContext& context)
{
    XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.position));
    XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotation));
    XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));

    worldMatrix = scale * translation * rotation;

    boundingbox.Center = transform.position;
}

void Model::UpdateAnimation(float time)
{
    //Skeleton skeleton = Importer::Data::GetSkeletonAt(mesh.sceneID, 0);

    //std::vector<XMMATRIX> localTransform(skeleton.jointCount);
    //std::vector<XMMATRIX> modelTransform(skeleton.jointCount);

    //for (int i = 0; i < skeleton.jointCount; ++i)
    //    localTransform[i] = FloatArrToMatrix(skeleton.joints[i].inverseBP) * worldMatrix;
}

Model::Model(const Mesh& mesh)
    :mesh(mesh), name(mesh.name)
{
    transform.position = { mesh.translation[0], mesh.translation[1], mesh.translation[2] };
    transform.rotation = { mesh.rotation[0], mesh.rotation[1], mesh.rotation[2] };
    transform.scale = { mesh.scale[0], mesh.scale[1], mesh.scale[2] };

    XMVECTOR quat = { mesh.rotation[0], mesh.rotation[1], mesh.rotation[2], mesh.rotation[3] };

    XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.position));
    XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotation));
    XMMATRIX quaternion = XMMatrixRotationQuaternion(quat);
    XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));

    worldMatrix = scale * quaternion * translation;

    XMFLOAT4 orientation;
    XMStoreFloat4(&orientation, XMQuaternionIdentity());
    boundingbox = BoundingOrientedBox(transform.position, { 2.0f, 2.0f, 2.0f }, orientation);
}

Model::Model()
    :worldMatrix(XMMatrixIdentity())
{}