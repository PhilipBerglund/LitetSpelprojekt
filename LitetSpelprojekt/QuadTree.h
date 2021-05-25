#pragma once
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"
#include "Event.h"

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

struct Plane
{
	XMFLOAT3 point;
	XMFLOAT3 normal;
};

class QTFrustum
{
public:
	
	Plane planes[4];
	XMFLOAT3 pos;
	XMMATRIX viewMatrix;

	QTFrustum();
	bool Contains(QTSquare bounds);
	void Update(Camera cam);
	void OnEvent();

	bool active = true;
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

