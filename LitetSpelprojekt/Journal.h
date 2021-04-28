#pragma once

#include "UI.h"
#include <vector>
#include "InputHandler.h"

class Journal
{
private:
	Image* image;
	
	float minX = 0;
	float maxX = 0;

	bool fullyShowing = false;
	bool fullyGone = true;

	bool slideIn = false;
	bool slideOut = false;

	double slideSpeed = 0.0000001;

	void SlideIn()
	{
		slideIn = true;
		slideOut = false;
	}

	void SlideOut()
	{
		slideOut = true;
		slideIn = false;
	}
public:
	Journal() { this->image = nullptr; };
	Journal(float windowWidth, float windowHeight)
	{
		image = new Image(L"UI/Journal.png", 1.0f, false, { windowWidth + 360, windowHeight / 2 });
		minX = windowWidth - 350;
		maxX = windowWidth + 360;
	}

	void Slide()
	{
		image->SetVisibility(true);

		if (fullyShowing)
			slideOut = true;
			
		else if (fullyGone)
			slideIn = true;

		else
		{
			if (slideIn)
				SlideOut();

			else if (slideOut)
				SlideIn();
		}

		fullyGone = false;
		fullyShowing = false;
	}

	void Draw()
	{
		if (slideIn)
		{
			double amount = pow(maxX - image->position.x, 2) * slideSpeed + 1;
			image->SetPosition(image->position.x - (float)amount, image->position.y);
			if (image->position.x <= minX)
			{
				fullyShowing = true;
				slideIn = false;
			}	
		}

		if (slideOut)
		{
			double amount = pow(maxX - image->position.x, 2) * slideSpeed + 1;
			image->SetPosition(image->position.x + (float)amount, image->position.y);
			if (image->position.x >= maxX)
			{
				fullyGone = true;
				slideOut = false;
				image->SetVisibility(false);
			}
		}

		if (image->visible)
			image->Draw();
	}
};