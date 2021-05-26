#pragma once
#include <DirectXMath.h>
#include <cmath>

using namespace DirectX;

inline XMMATRIX FloatArrToMatrix(float arr[])
{
	XMMATRIX matrix = { arr[0],		arr[1],		arr[2],		arr[3],
						arr[4],		arr[5],		arr[6],		arr[7], 
						arr[8],		arr[9],		arr[10],	arr[11],
						arr[12],	arr[13],	arr[14],	arr[15] };
	return matrix;
}

inline XMVECTOR FloatArrToQuat(float arr[])
{
	XMVECTOR quat = { arr[0], arr[1], arr[2], arr[3] };
	return quat;
}

inline XMVECTOR FloatArrToVec(float arr[])
{
	XMVECTOR vec = { arr[0], arr[1], arr[2] };
	return vec;
}

inline XMMATRIX VecsToMatrix(XMVECTOR scale, XMVECTOR rotation, XMVECTOR translation)
{
	XMMATRIX matrix = XMMatrixIdentity();
	XMMATRIX translationMat = XMMatrixTranslationFromVector(translation);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYawFromVector(rotation);
	//XMMATRIX rotationMat = XMMatrixRotationQuaternion(rotation);
	XMMATRIX scaleMat = XMMatrixScalingFromVector(scale);

	matrix = scaleMat * rotationMat * translationMat;

	return matrix;
}

inline XMMATRIX ArrayToMatrix(float array[])
{
	XMMATRIX matrix = { array[0],	array[1],	array[2],	array[3],
						array[4],	array[5],	array[6],	array[7],
						array[8],	array[9],	array[10],	array[11],
						array[12],	array[13],	array[14],	array[15] };

	return matrix;
}

inline XMMATRIX TransformToMatrix(float translation[], float rotation[], float scale[])
{
	XMVECTOR T = FloatArrToVec(translation);
	XMVECTOR R = FloatArrToVec(rotation);
	XMVECTOR S = FloatArrToVec(scale);

	return VecsToMatrix(S, R, T);
}