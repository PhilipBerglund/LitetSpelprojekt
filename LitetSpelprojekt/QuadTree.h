#pragma once
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"

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

class QTFrustum
{
public:
	
	XMFLOAT4 planes[4];

	XMMATRIX viewMatrix;

	QTFrustum();
	bool Contains(QTSquare bounds);
	void Update(Camera cam);
};


class QuadTree
{

public:

	int capacity;


	QuadTree* TopR = nullptr;
	QuadTree* TopL = nullptr;
	QuadTree* BotR = nullptr;
	QuadTree* BotL = nullptr;

	std::vector<std::shared_ptr<Model>> models;
	QTSquare boundary;
	bool divided = false;

public:

	QuadTree();
	QuadTree(QTSquare bounds, int cap);
	void InsertModel(std::shared_ptr<Model> model);
	void Divide();
	void DeleteQuadTree();
};


inline void QTIntersect(QTFrustum frust, QuadTree* tree, std::vector<std::shared_ptr<Model>> &foundModels)
{
	if (frust.Contains(tree->boundary) == true)
	{
		for (int i = 0; i < tree->models.size(); i++)
		{
			foundModels.push_back(tree->models[i]);
		}
	}

	if (tree->divided == true)
	{
		QTIntersect(frust, tree->TopR, foundModels);
		QTIntersect(frust, tree->TopL, foundModels);
		QTIntersect(frust, tree->BotR, foundModels);
		QTIntersect(frust, tree->BotL, foundModels);
	}
};



inline void SetupQuadTree(QuadTree* &QTree, QTSquare bounds, int capacity)
{
	QTree = new QuadTree(bounds, capacity);
};

inline void SetupFrustum(QTFrustum& frust, const Camera& cam)
{
	XMMATRIX pMatrix = cam.GetPerspectiveMatrix();
	XMMATRIX vMatrix = cam.GetViewMatrix();

	//Från 3D-GameProgramming
	//static XMVECTOR HomogenousPoints[6] =
	//{
	//{ 1.0f, 0.0f, 1.0f, 1.0f }, // right (at far plane)
	//{ -1.0f, 0.0f, 1.0f, 1.0f }, // left
	//{ 0.0f, 1.0f, 1.0f, 1.0f }, // top
	//{ 0.0f, -1.0f, 1.0f, 1.0f }, // bottom

	//{ 0.0f, 0.0f, 0.0f, 1.0f }, // near
	//{ 0.0f, 0.0f, 1.0f, 1.0f } // far
	//};

	//XMVECTOR pDeterminant;
	//XMMATRIX pMatrixInv = XMMatrixInverse(&pDeterminant, pMatrix);

};
