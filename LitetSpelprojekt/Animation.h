#pragma once

#include "Model.h"
#include <map>

struct Animator
{
private:
	float time = 0;
public:
	inline void PlayAnimation(float dt, Model& model)
	{
		time += dt;

		if (time > model.GetNumKeyFrames())
			time = 0;

		model.UpdateAnimation(time);
	}
};