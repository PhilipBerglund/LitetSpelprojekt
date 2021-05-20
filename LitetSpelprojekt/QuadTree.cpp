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
			if (crossProd.x < 0)
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
	this->viewMatrix = cam.GetViewMatrix();

	XMFLOAT4X4 pMatrix;
	XMStoreFloat4x4(&pMatrix, cam.GetPerspectiveMatrix());
	XMFLOAT4X4 vMatrix;
	XMStoreFloat4x4(&vMatrix, cam.GetViewMatrix());

	float zMin, r;

	//Beräkning av det minsta Z-värdet i frustrumet
	zMin = -pMatrix._43 / pMatrix._33;
	r = cam.getViewDistance() / (cam.getViewDistance() - zMin);
	pMatrix._33 = r;
	pMatrix._43 = -r * zMin;

	XMMATRIX ProjMatrix = XMLoadFloat4x4(&pMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&vMatrix);

	XMMATRIX fMatrix = XMMatrixMultiply(ProjMatrix, ViewMatrix);
	XMFLOAT4X4 frustMatrix;
	XMStoreFloat4x4(&frustMatrix, fMatrix);

	planes[0].x = frustMatrix._14 + frustMatrix._13; //Near Plane
	planes[0].y = frustMatrix._24 + frustMatrix._23;
	planes[0].z = frustMatrix._34 + frustMatrix._33;
	planes[0].w = frustMatrix._44 + frustMatrix._43;
	XMVECTOR plane0 = XMLoadFloat4(&planes[0]);
	plane0 = XMVector4Normalize(plane0);
	XMStoreFloat4(&planes[0], plane0);

	planes[1].x = frustMatrix._14 - frustMatrix._13; //Far Plane
	planes[1].y = frustMatrix._24 - frustMatrix._23;
	planes[1].z = frustMatrix._34 - frustMatrix._33;
	planes[1].w = frustMatrix._44 - frustMatrix._43;
	XMVECTOR plane1 = XMLoadFloat4(&planes[1]);
	plane1 = XMVector4Normalize(plane1);
	XMStoreFloat4(&planes[1], plane1);

	planes[2].x = frustMatrix._14 + frustMatrix._11; //Left Plane
	planes[2].y = frustMatrix._24 + frustMatrix._21;
	planes[2].z = frustMatrix._34 + frustMatrix._31;
	planes[2].w = frustMatrix._44 + frustMatrix._41;
	XMVECTOR plane2 = XMLoadFloat4(&planes[2]);
	plane2 = XMVector4Normalize(plane2);
	XMStoreFloat4(&planes[2], plane2);

	planes[3].x = frustMatrix._14 - frustMatrix._11; //Right Plane
	planes[3].y = frustMatrix._24 - frustMatrix._21;
	planes[3].z = frustMatrix._34 - frustMatrix._31;
	planes[3].w = frustMatrix._44 - frustMatrix._41;
	XMVECTOR plane3 = XMLoadFloat4(&planes[3]);
	plane3 = XMVector4Normalize(plane3);
	XMStoreFloat4(&planes[3], plane3);
}