#pragma once
#include <DirectXMath.h>
#include <map>
#include <vector>
#include <string>

using namespace DirectX;

struct Keyframe
{
	Keyframe();
	~Keyframe();

	float TimePos;
	XMFLOAT3 Translation;
	XMFLOAT3 Scale;
	XMFLOAT4 RotationQuat;
};

struct BoneAnimation
{
	float GetStartTime()const;
	float GetEndTime()const;

	void Interpolate(float t, XMFLOAT4X4& M)const;

	std::vector<Keyframe> Keyframe;
};

struct AnimationClip
{
	float GetClipStartTime()const;
	float GetClipEndTime()const;

	void Interpolate(float t,std::vector<XMFLOAT4X4>& boneTransform)const;

	std::vector<BoneAnimation> BoneAnimation;
};

class SkinnedData
{
private:
	std::vector<int> mBoneHierarchy;

	std::vector<XMFLOAT4X4> mBoneOffsets;

	std::map<std::string, AnimationClip> mAnimationsClip;
public:
	unsigned int BoneCount()const;

	float GetClipStartTime(const std::string& clipName)const;
	float GetClipEndTime(const std::string& clipName)const;

	void Set(std::vector<int>& boneHierarchy,std::vector<XMFLOAT4X4>& boneOffsets,
		     std::map<std::string, AnimationClip>& animations);

	void GetFinalTransforms(const std::string& clipName, float timePos,
		std::vector<XMFLOAT4X4>& finalTransforms)const;

};

