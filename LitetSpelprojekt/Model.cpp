#include "Model.h"
#include "Math.h"

void Model::Update(ID3D11DeviceContext& context)
{
    XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.position));
    XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotation));
    XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));

    worldMatrix = scale * rotation * translation;

    boundingbox.Center = transform.position;
}

void GetParentMatrix(Skeleton& skeleton, Joint& joint, int timeStamp, XMMATRIX& matrix)
{
    if (joint.parentID == -1)
        return;

    Joint parent = skeleton.joints.at(joint.parentID);

    XMVECTOR translation;
    XMVECTOR rotation;
    XMVECTOR scale;

    XMMATRIX mat;

    if (parent.animation.keyFrameCount != 0)
    {
        translation = FloatArrToVec(parent.animation.keyFrames[timeStamp].transform.translation);
        rotation = FloatArrToVec(parent.animation.keyFrames[timeStamp].transform.rotation);
        scale = FloatArrToVec(parent.animation.keyFrames[timeStamp].transform.scale);
        mat = VecsToMatrix(scale, rotation, translation);
    }

    else
    {
        mat = XMMatrixIdentity();
    }

    matrix = XMMatrixMultiply(matrix, mat);
    GetParentMatrix(skeleton, parent, timeStamp, matrix);
}

void Model::UpdateAnimation(float time)
{
    Skeleton skeleton = Importer::Data::GetSkeletonAt(mesh.sceneID, 0);
    jointTransforms.resize(skeleton.jointCount);

    XMVECTOR translation;
    XMVECTOR rotation;
    XMVECTOR scale;

    XMMATRIX rootTransform = XMMatrixIdentity();

    int timeStamp = round(time);

    Print(std::to_string(timeStamp));

    if (skeleton.joints[0].animation.keyFrameCount != 0)
    {
        translation = FloatArrToVec(skeleton.joints[0].animation.keyFrames[timeStamp].transform.translation);
        rotation = FloatArrToVec(skeleton.joints[0].animation.keyFrames[timeStamp].transform.rotation);
        scale = FloatArrToVec(skeleton.joints[0].animation.keyFrames[timeStamp].transform.scale);
    }

    else
    {
        translation = { 0,0,0 };
        rotation = { 0,0,0 };
        scale = { 1,1,1 };
    }

    rootTransform = VecsToMatrix(scale, rotation, translation);

    jointTransforms[0] = rootTransform * FloatArrToMatrix(skeleton.joints[0].inverseBP);

    for (int i = 1; i < skeleton.jointCount; ++i)
    {
        XMMATRIX jointTransform;

        if (skeleton.joints[i].animation.keyFrameCount != 0)
        {
            translation = FloatArrToVec(skeleton.joints[i].animation.keyFrames[timeStamp].transform.translation);
            rotation = FloatArrToVec(skeleton.joints[i].animation.keyFrames[timeStamp].transform.rotation);
            scale = FloatArrToVec(skeleton.joints[i].animation.keyFrames[timeStamp].transform.scale);
            jointTransform = VecsToMatrix(scale, rotation, translation);
        }

        else
        {
            jointTransform = XMMatrixIdentity();
        }

        XMMATRIX parentMatrix = XMMatrixIdentity();
        GetParentMatrix(skeleton, skeleton.joints[i], timeStamp, parentMatrix);
        jointTransforms[i] = parentMatrix * jointTransform * FloatArrToMatrix(skeleton.joints[i].inverseBP);
    }
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