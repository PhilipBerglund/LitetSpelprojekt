#include "SkinnedData.h"

Keyframe::Keyframe()
	: TimePos(0.0f), Translation(0.0f, 0.0f, 0.0f), Scale(1.0f, 1.0f, 1.0f), RotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Keyframe::~Keyframe()
{
}

float BoneAnimation::GetStartTime() const
{
	return Keyframe.front().TimePos;
}

float BoneAnimation::GetEndTime() const
{
	float f = Keyframe.back().TimePos;

	return f;
}

void BoneAnimation::Interpolate(float t, XMFLOAT4X4& M) const
{
	if( t <= Keyframe.front().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&Keyframe.front().Scale);
		XMVECTOR P = XMLoadFloat3(&Keyframe.front().Translation);
		XMVECTOR Q = XMLoadFloat4(&Keyframe.front().RotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else if ( t >= Keyframe.back().TimePos )
	{
		XMVECTOR S = XMLoadFloat3(&Keyframe.back().Scale);
		XMVECTOR P = XMLoadFloat3(&Keyframe.back().Translation);
		XMVECTOR Q = XMLoadFloat4(&Keyframe.back().RotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else
	{
		for(unsigned int i = 0; i < Keyframe.size()-1; i++)
		{
			if(t >= Keyframe[i].TimePos && t <= Keyframe[i+1].TimePos)
			{
				float lerpPercent = (t - Keyframe[i].TimePos) / (Keyframe[i + 1].TimePos - Keyframe[i].TimePos);
			
				XMVECTOR s0 = XMLoadFloat3(&Keyframe[i].Scale);
				XMVECTOR s1 = XMLoadFloat3(&Keyframe[i + 1].Scale);

				XMVECTOR p0 = XMLoadFloat3(&Keyframe[i].Translation);
				XMVECTOR p1 = XMLoadFloat3(&Keyframe[i + 1].Translation);

				XMVECTOR q0 = XMLoadFloat4(&Keyframe[i].RotationQuat);
				XMVECTOR q1 = XMLoadFloat4(&Keyframe[i + 1].RotationQuat);

				XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
				XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
				XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);

				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));

				break;		
			}		
		}	
	}
}

float AnimationClip::GetClipStartTime() const
{
	float t = std::numeric_limits<float>::min();
	for (unsigned int i = 0; i < BoneAnimation.size(); ++i)
	{
		t = fmin(t, BoneAnimation[i].GetStartTime());
	}

	return t;
}

float AnimationClip::GetClipEndTime() const
{
	float t = 0.0f;
	for (unsigned int i = 0; i < BoneAnimation.size(); ++i)
	{
		t = fmax(t, BoneAnimation[i].GetStartTime());
	}

	return t;
}

void AnimationClip::Interpolate(float t, std::vector<XMFLOAT4X4>& boneTransform) const
{
	for (unsigned int i = 0; i < BoneAnimation.size(); ++i)
	{
		BoneAnimation[i].Interpolate(t, boneTransform[i]);
	}
}

unsigned int SkinnedData::BoneCount() const
{
	return mBoneHierarchy.size();
}

float SkinnedData::GetClipStartTime(const std::string& clipName) const
{
	auto clip = mAnimationsClip.find(clipName);
	return clip->second.GetClipStartTime();
}

float SkinnedData::GetClipEndTime(const std::string& clipName) const
{
	auto clip = mAnimationsClip.find(clipName);
	return clip->second.GetClipEndTime();
}

void SkinnedData::Set(std::vector<int>& boneHierarchy, std::vector<XMFLOAT4X4>& boneOffsets, 
	std::map<std::string, AnimationClip>& animations)
{
	mBoneHierarchy = boneHierarchy;
	mBoneOffsets = boneOffsets;
	mAnimationsClip = animations;
}

void SkinnedData::GetFinalTransforms(const std::string& clipName, float timePos, std::vector<XMFLOAT4X4>& finalTransforms) const
{
	unsigned int numBones = mBoneOffsets.size();

	std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	// Interpolate all the bones of this clip at the given time instance.
	auto clip = mAnimationsClip.find(clipName);
	clip->second.Interpolate(timePos, toParentTransforms);

	//
	// Traverse the hierarchy and transform all the bones to the root space.
	//

	std::vector<XMFLOAT4X4> toRootTransforms(numBones);

	// The root bone has index 0.  The root bone has no parent, so its toRootTransform
	// is just its local bone transform.
	toRootTransforms[0] = toParentTransforms[0];

	// Now find the toRootTransform of the children.
	for (unsigned int i = 1; i < numBones; ++i)
	{
		XMMATRIX toParent = XMLoadFloat4x4(&toParentTransforms[i]);

		int parentIndex = mBoneHierarchy[i];
		XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

		XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	}

	// Premultiply by the bone offset transform to get the final transform.
	for (unsigned int i = 0; i < numBones; ++i)
	{
		XMMATRIX offset = XMLoadFloat4x4(&mBoneOffsets[i]);
		XMMATRIX toRoot = XMLoadFloat4x4(&toRootTransforms[i]);
		XMStoreFloat4x4(&finalTransforms[i], XMMatrixMultiply(offset, toRoot));
	}
}
