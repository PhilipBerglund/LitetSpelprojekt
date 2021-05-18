#pragma once
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"

class QTFrustum
{
public:
	XMFLOAT3 Origin;
	XMFLOAT4 Orientation;

	float RightSlope;
	float LeftSlope;
	float TopSlope;
	float BottomSlope;
	float NearPlane;
	float FarPlane;

	QTFrustum();
};

struct QTPoint
{
	float x;
	float z;
};

struct QTSquare
{

	float xPos;
	float zPos;
	float w;
	float h;

	bool contains(QTPoint p)
	{
		return(p.x <= this->xPos + this->w &&
				p.x >= this->xPos - this->w &&
				p.z <= this->zPos + this->h &&
				p.z >= this->zPos - this->h);
	};

};

class QuadTree
{

private:

	QTSquare boundary;
	int capacity;
	std::vector<std::shared_ptr<Model>> models;

	bool divided = false;

	QuadTree* TopR = nullptr;
	QuadTree* TopL = nullptr;
	QuadTree* BotR = nullptr;
	QuadTree* BotL = nullptr;


public:
	QuadTree();
	QuadTree(QTSquare bounds, int cap);
	void InsertModel(std::shared_ptr<Model> model);
	void Divide();
	void DeleteQuadTree();
};


//void QTIntersect(QTFrustum frust, QuadTree tree)
//{
//	
//};



inline void SetupQuadTree(QuadTree* &QTree, QTSquare bounds, int capacity)
{
	QTree = new QuadTree(bounds, capacity);
};

inline void SetupFrustum(QTFrustum& frust, const Camera& cam)
{
	XMMATRIX pMatrix = cam.GetPerspectiveMatrix();

	//Från 3D-GameProgramming
	static XMVECTOR HomogenousPoints[6] =
	{
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // right (at far plane)
	{ -1.0f, 0.0f, 1.0f, 1.0f }, // left
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // top
	{ 0.0f, -1.0f, 1.0f, 1.0f }, // bottom

	{ 0.0f, 0.0f, 0.0f, 1.0f }, // near
	{ 0.0f, 0.0f, 1.0f, 1.0f } // far
	};

	XMVECTOR pDeterminant;
	XMMATRIX pMatrixInv = XMMatrixInverse(&pDeterminant, pMatrix);

	XMVECTOR frustumPoints[6];
	for (int i = 0; i < 6; i++)
	{
		frustumPoints[i] = XMVector4Transform(HomogenousPoints[i], pMatrixInv);
	}

	frust.Origin = cam.GetPosition();
	frust.Orientation = { 0.0 ,0.0 , 0.0, 1.0f };

	//Beräkna de lutande kanterna
	for (int i = 0; i < 4; i++)
	{
		frustumPoints[i] = frustumPoints[i] * XMVectorReciprocal(XMVectorSplatZ(frustumPoints[i]));
	}

	frust.RightSlope = XMVectorGetX(frustumPoints[0]);
	frust.LeftSlope = XMVectorGetX(frustumPoints[0]);
	frust.TopSlope = XMVectorGetY(frustumPoints[0]);
	frust.BottomSlope = XMVectorGetY(frustumPoints[0]);

	frustumPoints[4] = frustumPoints[4] * XMVectorReciprocal(XMVectorSplatW(frustumPoints[4]));
	frustumPoints[5] = frustumPoints[5] * XMVectorReciprocal(XMVectorSplatW(frustumPoints[5]));

	frust.NearPlane = XMVectorGetZ(frustumPoints[4]);
	frust.FarPlane = XMVectorGetZ(frustumPoints[5]);
};
