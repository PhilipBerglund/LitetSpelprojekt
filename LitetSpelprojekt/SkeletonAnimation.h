//#pragma once
//#include <DirectXMath.h>
//#include <d3d11.h>
//#include "Windowsnumerics.h"
//
//using namespace DirectX;
//using namespace Windows::Foundation::Numerics;
//
//static const int BONE_COUNT = 50;		// Ska vara hur många ben som finns med i skelettet
//
//struct Skeleton 
//{
//
//	int parentArr[BONE_COUNT];				// Parent of each bone
//	float4x4 invBindTxArr[BONE_COUNT];	// Inverse of original model transform in bind pose
//	float4x4 modelTxArr[BONE_COUNT];	    // (dynamic) Current model transform
//	float4x4 localTxArr[BONE_COUNT];	    // (dynamic) Current local transform
//	float4x4 finalTxArr[BONE_COUNT];		// (dynamic) What to upload to GPU
//
//	void startUp();
//	void perFrame();
//
//};