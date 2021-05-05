#pragma once

#include "Event.h"
#include "GameSettings.h"
#include "UI.h"
#include "Window.h"

enum class JournalState { OVERVIEW, PAGE };

struct Page
{
	unsigned int ID = -1;

	Image img;

	Text name;
	Text age;
	Text height;
	Text shoesize;

	Text characteristicsHeading;
	Text characteristics[3];

	Text rumorsHeading;
	Text rumors[3];

	Text markAsPrime;
	Image primeCheckbox;

	Text MarkAsInnocent;
	Image innocentCheckbox;

	Page()
	{
		D2D_VECTOR_2F winSize = { Window::GetWidth(), Window::GetHeight() };

		float fontSize = 35;
		float leftBorder = winSize.x + 350;
		float startHeight = 250;

		img = Image(L"UI/Unknown.png", 1.3f, true, { winSize.x + 550, 350 });

		name.position = { leftBorder - name.GetSize() *  fontSize, startHeight };

		age.position = { leftBorder - age.GetSize() * fontSize, startHeight + 50 };
		height.position = { leftBorder - height.GetSize() * fontSize, startHeight + 100 };
		shoesize.position = { leftBorder - shoesize.GetSize() * fontSize , startHeight + 150};

		characteristicsHeading = Text(L"Characteristics:", true, {leftBorder - characteristicsHeading.GetSize() * fontSize, startHeight + 250}, 500);
	};

	void Move(float x, float y = 0)
	{
		name.SetPosition(name.position.x + x, name.position.y);
		age.SetPosition(age.position.x + x, age.position.y);
		height.SetPosition(height.position.x + x, height.position.y);
		shoesize.SetPosition(shoesize.position.x + x, shoesize.position.y);

		img.SetPosition(img.position.x + x, img.position.y);

		characteristicsHeading.SetPosition(characteristicsHeading.position.x + x, characteristicsHeading.position.y);
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush)
	{
		img.Draw();
		name.Draw(format, brush);
		age.Draw(format, brush);
		height.Draw(format, brush);
		shoesize.Draw(format, brush);
		characteristicsHeading.Draw(format, brush);
	}
};

struct Slot
{
	D2D_VECTOR_2F position = {};
	unsigned int ID;
	Button button;
	Image image;
	Text name;

	Slot() = default;
	Slot(std::wstring text, LPCWSTR path, D2D_VECTOR_2F position, unsigned int ID)
		:position(position), ID(ID)
	{
		image = Image(path, 1.0f, true, position);
		name = Text(text, true, position, image.width + 50);
		button = Button(true, position, image.width, image.height + 40);
	}

	void SetPosition(float x, float y)
	{
		position = { x, y };
		image.SetPosition(x, y);
		name.SetPosition(x, y + 80);
		button.SetPosition(x, y + 20);
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush)
	{
		image.Draw();
		name.Draw(format, brush);
	}
};

class Journal
{
private:
	JournalState state;

	bool exit = false;

	Image journalBase;
	Image exitCross;

	Button exitButton;

	unsigned int openedSlot = 0;
	unsigned int filledSlots = 0;
	Page pages[9];
	Slot slots[9];
	D2D_VECTOR_2F slotPositions[9];

	//TEXT STUFF
	ComPtr<IDWriteTextFormat> textFormat;
	std::wstring font = L"Gabriola";
	FLOAT fontSize = 35;

	float minX = 0;
	float maxX = 0;

	bool fullyShowing = false;
	bool fullyGone = true;

	bool slideIn = false;
	bool slideOut = false;

	double slideSpeed = 10.0f;

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
	Journal()
	{
		Event::Bind(this, EventType::LEFTCLICK);

		journalBase = Image(L"UI/Journal.png", 1.0f, false, { (float)Window::GetWidth() + 360, (float)Window::GetHeight() / 2 });

		exitCross = Image(L"UI/ExitCross.png", 1.0f, true, { journalBase.position.x - 240, journalBase.position.y - 340});
		exitButton = Button(true, { exitCross.position.x - 10 ,exitCross.position.y }, 50, 40);

		slotPositions[0] = { journalBase.position.x - 180, journalBase.position.y - 200 };
		slotPositions[1] = { journalBase.position.x + 20, journalBase.position.y - 200 };
		slotPositions[2] = { journalBase.position.x + 220, journalBase.position.y - 200 };

		slotPositions[3] = { journalBase.position.x - 180, journalBase.position.y + 25 };
		slotPositions[4] = { journalBase.position.x + 20, journalBase.position.y + 25 };
		slotPositions[5] = { journalBase.position.x + 220, journalBase.position.y + 25 };

		slotPositions[6] = { journalBase.position.x - 180, journalBase.position.y + 250 };
		slotPositions[7] = { journalBase.position.x + 20, journalBase.position.y + 250 };
		slotPositions[8] = { journalBase.position.x + 220, journalBase.position.y + 250 };

		for (unsigned int i = 0; i < 9; ++i)
			slots[i] = Slot(L"Unknown", L"UI/Unknown.png", slotPositions[i], i);

		minX = (float)Window::GetWidth() - 350;
		maxX = (float)Window::GetWidth() + 360;

		//TEXT
		HRESULT hr = Graphics::GetWriteFactory().CreateTextFormat(font.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_ITALIC, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &textFormat);
		if FAILED(hr)
		{
			Error("FAILED TO CREATE TEXT FORMAT");
			return;
		}
	}

	//RESET JOURNAL INFORMATION
	void Reset()
	{

	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush, float dt)
	{
		if (slideIn)
		{
			float distance = maxX - journalBase.position.x;
			float amount = slideSpeed / ((distance + 100.0f) * dt);
			journalBase.SetPosition(journalBase.position.x - (float)amount, journalBase.position.y);
			exitCross.SetPosition(exitCross.position.x - (float)amount, exitCross.position.y);
			exitButton.SetPosition(exitButton.position.x - (float)amount, exitButton.position.y);

			for (auto& page : pages)
				page.Move(-(float)amount);

			for (auto& slot : slots)
				slot.SetPosition(slot.position.x - (float)amount, slot.position.y);

			if (journalBase.position.x <= minX)
			{
				fullyShowing = true;
				slideIn = false;
			}
		}

		if (slideOut)
		{
			float distance = maxX - journalBase.position.x;
			float amount = slideSpeed / ((distance + 100.0f) * dt);
			journalBase.SetPosition(journalBase.position.x + (float)amount, journalBase.position.y);
			exitCross.SetPosition(exitCross.position.x + (float)amount, exitCross.position.y);
			exitButton.SetPosition(exitButton.position.x + (float)amount, exitButton.position.y);
			
			for (auto& page : pages)
				page.Move((float)amount);

			for (auto& slot : slots)
				slot.SetPosition(slot.position.x + (float)amount, slot.position.y);

			if (journalBase.position.x >= maxX)
			{
				fullyGone = true;
				slideOut = false;
				journalBase.SetVisibility(false);
			}
		}

		if (journalBase.visible)
		{
			journalBase.Draw();
			exitCross.Draw();

			if (state == JournalState::OVERVIEW)
			{
				for (auto& slot : slots)
					slot.Draw(format, brush);
			}
			
			if (state == JournalState::PAGE)
			{
				pages[openedSlot].Draw(*textFormat.Get(), brush);
			}
		}
	}

	void OnEvent()
	{
		if (GameSettings::GetState() != GameState::JOURNAL)
			return;

		std::pair<int, int> pos = Window::GetMousePos();

		if (state == JournalState::OVERVIEW)
		{
			if (this->exitButton.OnClick(pos.first, pos.second))
			{
				exit = true;
			}

			for (auto& slot : slots)
			{
				if (slot.button.OnClick(pos.first, pos.second) && pages[slot.ID].ID != -1)
				{
					state = JournalState::PAGE;
					openedSlot = slot.ID;
				}
			}
		}
		
		if (state == JournalState::PAGE)
		{
			if (this->exitButton.OnClick(pos.first, pos.second))
			{
				state = JournalState::OVERVIEW;
			}
		}
	}

	void AddSuspect(unsigned int ID, std::string name, bool interacted, int age = 0, int height = 0, int shoeSize = 0)
	{
		std::wstring temp(name.begin(), name.end());

		slots[ID].ID = ID;
		slots[ID].name = Text(temp, true, slots[ID].name.position, 500);

		pages[ID].ID = ID;
		pages[ID].name = Text(L"Name: " + temp, true, pages[ID].name.position, 500);

		if (interacted)
		{

		}

		else
		{
			pages[ID].age = Text(L"Age: XX", true, pages[ID].age.position, 500);
			pages[ID].height = Text(L"Height: XXX", true, pages[ID].height.position, 500);
			pages[ID].shoesize = Text(L"Shoesize: XX", true, pages[ID].shoesize.position, 500);
		}
	}

	void UpdateSuspect(unsigned int ID, int age = 0, int height = 0, int shoeSize = 0)
	{

	}

	void Slide()
	{
		if (exit)
			exit = false;

		journalBase.SetVisibility(true);

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

	bool Exit()	{	return exit;	}
};