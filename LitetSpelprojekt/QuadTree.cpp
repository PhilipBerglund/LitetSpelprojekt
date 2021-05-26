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
	Event::Bind(this, EventType::LEFTCLICK);
}


bool QTFrustum::Contains(QTSquare bounds)
{
	XMVECTOR corners[5];
	corners[0] = { bounds.xPos + bounds.w, 0, bounds.zPos + bounds.h }; //TopRight
	corners[1] = { bounds.xPos - bounds.w, 0, bounds.zPos + bounds.h }; //TopLeft
	corners[2] = { bounds.xPos + bounds.w, 0, bounds.zPos - bounds.h }; //BottomRight
	corners[3] = { bounds.xPos - bounds.w, 0, bounds.zPos - bounds.h }; //BottomLeft
	corners[4] = { bounds.xPos, 0, bounds.zPos };						//CenterPoint

	for (int i = 0; i < 5; i++) //Points
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

	if (this->pos.x < bounds.xPos + bounds.w && this->pos.x > bounds.xPos - bounds.w
		&& this->pos.z < bounds.zPos + bounds.h && this->pos.z > bounds.zPos - bounds.h)
	{
		return true;
	}

	return false;
}

void QTFrustum::Update(Camera cam)
{
	/*if (!active)
		return;*/


	this->pos = cam.GetPosition();

	XMFLOAT3 CamForwardV;
	XMFLOAT3 CamForwardVNegative;
	XMStoreFloat3(&CamForwardV, DirectX::XMVector3Normalize(cam.GetFForwardVector()));	//Spara normaliserad kameras fram-vektor i float3
	XMStoreFloat3(&CamForwardVNegative, DirectX::XMVector3Normalize(XMVectorNegate(cam.GetFForwardVector()))); ////Spara Negerad normaliserad kameras fram-vektor i float3
	XMVECTOR CamUpV = cam.GetUpVector();
	XMVector3Normalize(CamUpV);
	XMFLOAT3 CamPos = cam.GetPosition();
	XMVECTOR CamPosVector = XMLoadFloat3(&CamPos);

	float nearZ = cam.GetNearZ();
	float farZ = cam.GetFarZ();

	float nearH = XMScalarCos(cam.GetFov()) / XMScalarSin(cam.GetFov());
	float nearW = nearH * cam.GetRatio();

	float farH = 2 * ((XMScalarSin(cam.GetFov() / 2)) / (XMScalarCos(cam.GetFov() / 2))) * farZ;
	float farW = farH * cam.GetRatio();
 

	XMVECTOR fPoint = CamPosVector + cam.GetFForwardVector() * farZ;
	XMStoreFloat3(&this->planes[0].point, fPoint);
	XMVECTOR nPoint = CamPosVector + cam.GetFForwardVector() * nearZ;
	XMStoreFloat3(&this->planes[1].point, nPoint);

	this->planes[2].point = CamPos;	//Right Plane
	this->planes[3].point = CamPos;	//Left Plane

	this->planes[0].normal = CamForwardV; //Hämta kamerans forward vector //Far Plane
	this->planes[1].normal = CamForwardVNegative; //Negera kamerans forward vector //Near Plane


	XMVECTOR rightPoint = CamPosVector + cam.GetFForwardVector() * farZ + CamUpV * (farH / 2.0f) + cam.GetFRightVector() * (farW / 2.0f);
	XMVECTOR leftPoint = CamPosVector + cam.GetFForwardVector() * farZ + CamUpV * (farH / 2.0f) - cam.GetFRightVector() * (farW / 2.0f);

	XMVECTOR rightPlaneNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(CamUpV, DirectX::XMVector3Normalize(rightPoint - CamPosVector))); //Hämta kamerans uppvektor, gör en vektor till punkten rightPoint i högra planet
	XMVECTOR leftPlaneNormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(leftPoint - CamPosVector), CamUpV)); //Hämta kamerans uppvektor, gör en vektor till punkten leftPoint i vänstra planet //Tänka på ordning i crossproduct


	XMStoreFloat3(&this->planes[2].normal, rightPlaneNormal);
	XMStoreFloat3(&this->planes[3].normal, leftPlaneNormal);
	
}

void QTFrustum::OnEvent()
{

	/*if (active)
		active = false;
	else
		active = true;*/

}
