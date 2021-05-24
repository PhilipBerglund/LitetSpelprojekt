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

}


bool QTFrustum::Contains(QTSquare bounds)
{
	XMVECTOR corners[4];
	corners[0] = { bounds.xPos + bounds.w, 0, bounds.zPos + bounds.h }; //TopRight
	corners[1] = { bounds.xPos - bounds.w, 0, bounds.zPos + bounds.h }; //TopLeft
	corners[2] = { bounds.xPos + bounds.w, 0, bounds.zPos - bounds.h }; //BottomRight
	corners[3] = { bounds.xPos - bounds.w, 0, bounds.zPos - bounds.h }; //BottomLeft
	for (int i = 0; i < 4; i++)
	{
		corners[i] = XMVector3Transform(corners[i], this->viewMatrix);
	}

	XMVECTOR planes[4];
	planes[0] = { this->planes[0].x, this->planes[0].y, this->planes[0].z};
	planes[1] = { this->planes[1].x, this->planes[1].y, this->planes[1].z};
	planes[2] = { this->planes[2].x, this->planes[2].y, this->planes[2].z};
	planes[3] = { this->planes[3].x, this->planes[3].y, this->planes[3].z};
	
	for (int i = 0; i < 4; i++) //Points
	{
		bool pInFront[4];

		for (int j = 0; j < 4; j++) //Planes
		{
			XMVECTOR crossP = XMPlaneDotCoord(planes[j], corners[i]);
			XMVECTOR length = XMVector3Length(crossP);
			XMFLOAT3 crossProd;
			XMStoreFloat3(&crossProd, length);
			if (crossProd.x > 0)
			{
				pInFront[j] = false;
			}
			else
			{
				pInFront[j] = true;
			}
		}

		if (pInFront[0] == true && pInFront[1] == true && pInFront[2] == true && pInFront[3] == true)
		{
			return true;
		}
	}
	return false;
}

void QTFrustum::Update(Camera cam)
{
	XMFLOAT3 CamForwardV;
	XMFLOAT3 CamForwardVNegative;
	XMStoreFloat3(&CamForwardV, DirectX::XMVector3Normalize(cam.GetForwardVector()));	//Spara normaliserad kameras fram-vektor i float3
	XMStoreFloat3(&CamForwardVNegative, DirectX::XMVector3Normalize(XMVectorNegate(cam.GetForwardVector()))); ////Spara Negerad normaliserad kameras fram-vektor i float3
	XMFLOAT3 CamUpV;
	XMStoreFloat3(&CamUpV, DirectX::XMVector3Normalize(cam.GetUpVector()));

	this->planes[2].point = cam.GetPosition();	//Right Plane
	this->planes[3].point = cam.GetPosition();	//Left Plane



	this->planes[0].normal = CamForwardV; //Hämta kamerans forward vector //Far Plane
	this->planes[1].normal = CamForwardVNegative; //Negera kamerans forward vector //Near Plane

	this->planes[3].normal = DirectX::XMVector3Cross(DirectX::XMVector3Normalize(cam.GetUpVector(), rightPoint - cam.GetPosition())) //Hämta kamerans uppvektor, gör en vektor till punkten rightPoint i högra planet
	this->planes[3].normal = DirectX::XMVector3Cross(DirectX::XMVector3Normalize(leftPoint - cam.GetPosition(), cam.GetUpVector())) //Hämta kamerans uppvektor, gör en vektor till punkten leftPoint i vänstra planet //Tänka på ordning i crossproduct
}