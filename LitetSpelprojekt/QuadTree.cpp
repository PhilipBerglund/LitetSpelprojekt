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

	for (int i = 0; i < 4; i++) //Points
	{
		bool inside[4];
		for (int i = 0; i < 4; i++)
		{
			inside[i] = false;
		}
		for (int j = 0; j < 4; j++) //Planes
		{
			XMVECTOR planeNormal = XMLoadFloat3(&this->planes[j].normal);
			XMVECTOR planePoint = XMLoadFloat3(&this->planes[j].point);
			XMVECTOR dVec = XMVector3Dot((corners[i] - planePoint), planeNormal);
			float d = XMVectorGetX(dVec);
			if (d < 0)
			{
				inside[j] = true;
			}
		}
		if (inside[0] == true && inside[1] == true && inside[2] == true && inside[3] == true)
		{
			return true;
		}
	}

	return false;
}

void QTFrustum::Update(Camera cam)
{
	this->pos = cam.GetPosition();

	XMFLOAT3 CamForwardV;
	XMFLOAT3 CamForwardVNegative;
	XMStoreFloat3(&CamForwardV, DirectX::XMVector3Normalize(cam.GetForwardVector()));	//Spara normaliserad kameras fram-vektor i float3
	XMStoreFloat3(&CamForwardVNegative, DirectX::XMVector3Normalize(XMVectorNegate(cam.GetForwardVector()))); ////Spara Negerad normaliserad kameras fram-vektor i float3
	XMVECTOR CamUpV = cam.GetUpVector();
	XMVector3Normalize(CamUpV);
	XMFLOAT3 CamPos = cam.GetPosition();
	XMVECTOR CamPosVector = XMLoadFloat3(&CamPos);

	float nearZ = cam.GetNearZ();
	float farZ = cam.GetFarZ();

	float nearH = XMScalarCos(cam.GetFov()) / XMScalarSin(cam.GetFov());
	float nearW = nearH / cam.GetRatio();
 

	XMVECTOR fPoint = CamPosVector + cam.GetForwardVector() * farZ;
	XMStoreFloat3(&this->planes[0].point, fPoint);
	XMVECTOR nPoint = CamPosVector + cam.GetForwardVector() * nearZ;
	XMStoreFloat3(&this->planes[1].point, nPoint);

	this->planes[2].point = CamPos;	//Right Plane
	this->planes[3].point = CamPos;	//Left Plane

	this->planes[0].normal = CamForwardV; //Hämta kamerans forward vector //Far Plane
	this->planes[1].normal = CamForwardVNegative; //Negera kamerans forward vector //Near Plane


	XMVECTOR rightPoint = CamPosVector + cam.GetForwardVector() * nearZ + CamUpV * (nearH / 2) + cam.GetRightVector() * (nearW / 2);
	XMVECTOR leftPoint = CamPosVector + cam.GetForwardVector() * nearZ + CamUpV * (nearH / 2) + cam.GetRightVector() * (-nearW / 2);

	XMVECTOR rightPlaneNormal = DirectX::XMVector3Cross(DirectX::XMVector3Normalize(CamUpV), rightPoint - CamPosVector); //Hämta kamerans uppvektor, gör en vektor till punkten rightPoint i högra planet
	XMVECTOR leftPlaneNormal = DirectX::XMVector3Cross(leftPoint - CamPosVector, DirectX::XMVector3Normalize(CamUpV)); //Hämta kamerans uppvektor, gör en vektor till punkten leftPoint i vänstra planet //Tänka på ordning i crossproduct


	XMStoreFloat3(&this->planes[2].normal, rightPlaneNormal);
	XMStoreFloat3(&this->planes[3].normal, leftPlaneNormal);

}