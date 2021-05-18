#include "QuadTree.h"

QuadTree::QuadTree()
{
}

QuadTree::QuadTree(QTSquare bounds, int cap)
{
	this->boundary = bounds;
	this->capacity = cap;
}

void QuadTree::InsertModel(std::shared_ptr<Model> model)
{

	QTPoint centerPoint;
	centerPoint.x = model->GetPosition().x;
	centerPoint.z = model->GetPosition().z;

	
	if (boundary.contains(centerPoint))
	{
		if (models.size() < capacity)
		{
			models.push_back(model);
		}
		else
		{
			if (divided == false)
			{
				Divide();
			}

			TopR->InsertModel(model);
			TopL->InsertModel(model);
			BotR->InsertModel(model);
			BotL->InsertModel(model);
		}
	}

}



void QuadTree::Divide()
{
	QTSquare TR;
	TR.xPos = this->boundary.xPos + this->boundary.w / 2;
	TR.zPos = this->boundary.zPos + this->boundary.h / 2;
	TR.w = this->boundary.w / 2;
	TR.h = this->boundary.h / 2;

	QTSquare TL;
	TL.xPos = this->boundary.xPos - this->boundary.w / 2;
	TL.zPos = this->boundary.zPos + this->boundary.h / 2;
	TL.w = this->boundary.w / 2;
	TL.h = this->boundary.h / 2;

	QTSquare BR;
	BR.xPos = this->boundary.xPos + this->boundary.w / 2;
	BR.zPos = this->boundary.zPos - this->boundary.h / 2;
	BR.w = this->boundary.w / 2;
	BR.h = this->boundary.h / 2;

	QTSquare BL;
	BL.xPos = this->boundary.xPos - this->boundary.w / 2;
	BL.zPos = this->boundary.zPos - this->boundary.h / 2;
	BL.w = this->boundary.w / 2;
	BL.h = this->boundary.h / 2;

	this->TopR = new QuadTree(TR, this->capacity);
	this->TopL = new QuadTree(TL, this->capacity);
	this->BotR = new QuadTree(BR, this->capacity);
	this->BotL = new QuadTree(BL, this->capacity);

	this->divided = true;

	return;
}


void QuadTree::DeleteQuadTree()
{
	if (this->TopR->divided == true)
	{
		this->DeleteQuadTree();
	}
	if (this->TopL->divided == true)
	{
		this->DeleteQuadTree();
	}
	if (this->BotR->divided == true)
	{
		this->DeleteQuadTree();
	}
	if (this->BotR->divided == true)
	{
		this->DeleteQuadTree();
	}

	delete this;
}

QTFrustum::QTFrustum()
{
	this->Origin = {0,0,0};
	this->Orientation = {0,0,0,0};

	this->RightSlope = 0;
	this->LeftSlope = 0;
	this->TopSlope = 0;
	this->BottomSlope = 0;
	this->NearPlane = 0;
	this->FarPlane = 0;
}
