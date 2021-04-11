#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

struct Skeleton {
	static const int BONE_COUNT = 50;		// Ska vara

	int parentArr[BONE_COUNT];				// Parent of each bone
	XMFLOAT4X4 invBindTxArr[BONE_COUNT];	// Inverse of original model transform in bind pose
	XMFLOAT4X4 modelTxArr[BONE_COUNT];	    // (dynamic) Current model transform
	XMFLOAT4X4 localTxArr[BONE_COUNT];	    // (dynamic) Current local transform
	XMFLOAT4X4 finalTxArr[BONE_COUNT];		// (dynamic) What to upload to GPU
};