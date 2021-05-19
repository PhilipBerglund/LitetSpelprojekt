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

		model.UpdateAnimation(time);

		if (time >= 25)
			time = 0;
	}
};