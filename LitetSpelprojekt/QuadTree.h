#pragma once
#include "Model.h"
#include <vector>

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

};

class QuadTree
{

private:

	QTSquare boundary;
	int capacity;
	std::vector<std::shared_ptr<Model>> models;

	QuadTree* TopR = nullptr;
	QuadTree* TopL = nullptr;
	QuadTree* BotR = nullptr;
	QuadTree* BotL = nullptr;

public:
	QuadTree();
	QuadTree(QTSquare bounds, int cap);
	void InsertModel(std::shared_ptr<Model> model);
	void Divide();

};



void SetupQuadTree()
{
	QTSquare start;
	start.h = 600;
	start.w = 600;
	start.xPos = 0;
	start.zPos = 0;

	QuadTree Tree(start, 10);
};