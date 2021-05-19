#pragma once

#include "Model.h"
#include <map>

struct Animator
{
private:
	float time;
public:
	inline void PlayAnimation(float dt, Model& model)
	{
		time += dt;

		model.UpdateAnimation(time);
	}
};