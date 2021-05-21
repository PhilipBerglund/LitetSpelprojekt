#pragma once
#include <DirectXCollision.h>
#include "Importer.h"
#include "Graphics.h"

using namespace DirectX;

struct Bounds
{
	std::vector<BoundingBox> boxes;

	Bounds() = default;
	Bounds(std::string path)
	{
		Importer::LoadScene(path);
		Importer::Initialize(Graphics::GetDevice());

		std::vector<Mesh> meshes = Importer::Data::GetMeshes(Importer::Data::scenes.size() - 1);

		for (auto& mesh : meshes)
		{
			BoundingBox bbox;

			XMFLOAT3 center = { mesh.translation[0], mesh.translation[1], mesh.translation[2] };
			float xMin, xMax, yMin, yMax, zMin, zMax;
			xMin = xMax = yMin = yMax = zMin = zMax = 0;

			for (auto& vertex : Importer::Data::GetVertexBufferAt(mesh.sceneID, mesh.vertexBufferID, true).vertices)
			{
				XMVECTOR vertexPos = { vertex.position[0], vertex.position[1], vertex.position[2] };

				XMVECTOR translation = { mesh.translation[0], mesh.translation[1], mesh.translation[2] };
				XMVECTOR rotation = { mesh.rotation[0], mesh.rotation[1], mesh.rotation[2], mesh.rotation[3] };
				XMVECTOR scale = { mesh.scale[0], mesh.scale[1], mesh.scale[2] };

				XMMATRIX worldMatrix = XMMatrixScalingFromVector(scale) * XMMatrixRotationQuaternion(rotation) * XMMatrixTranslationFromVector(translation);

				vertexPos = XMVector4Transform(vertexPos, worldMatrix);
				XMFLOAT3 vPos;
				XMStoreFloat3(&vPos, vertexPos);

				float x = vPos.x;
				float y = vPos.y;
				float z = vPos.z;

				if (x < xMin)
					xMin = x;

				if (x > xMax)
					xMax = x;

				if (y < yMin)
					yMin = y;

				if (y > yMax)
					yMax = y;

				if (z < zMin)
					zMin = z;

				if (z > zMax)
					zMax = z;
			}

			float xExtent = (xMax - xMin) / 2.0f;
			float yExtent = (yMax - yMin) / 2.0f;
			float zExtent = (zMax - zMin) / 2.0f;

			boxes.push_back(BoundingBox(center, { xExtent, yExtent, zExtent }));
		}
	}
};