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
	Image profile;

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
		float startHeight = 230;

		img = Image(L"./UI/Unknown.png", 1.3f, true, { winSize.x + 550, 350 });
		profile = Image(L"./UI/Unknown.png", 1.0f, false, img.position);

		name.position = { leftBorder - name.GetSize() *  fontSize, startHeight };

		age.position = { leftBorder - age.GetSize() * fontSize, startHeight + 50 };
		height.position = { leftBorder - height.GetSize() * fontSize, startHeight + 100 };
		shoesize.position = { leftBorder - shoesize.GetSize() * fontSize , startHeight + 150};

		characteristicsHeading = Text(L"", true, {leftBorder - characteristicsHeading.GetSize() * fontSize, startHeight + 240}, 500);
		characteristics[0].position = { leftBorder - characteristics[0].GetSize() * fontSize, startHeight + 280 };
		characteristics[1].position = { leftBorder - characteristics[1].GetSize() * fontSize, startHeight + 320 };
		characteristics[2].position = { leftBorder - characteristics[2].GetSize() * fontSize, startHeight + 360 };

		rumorsHeading.position = { leftBorder - rumorsHeading.GetSize() * fontSize, startHeight + 430 };

		rumors[0].position = { leftBorder - (rumors[0].GetSize() * fontSize), startHeight + 470 };
		rumors[1].position = { leftBorder - (rumors[1].GetSize() * fontSize), startHeight + 520 };
		rumors[2].position = { leftBorder - (rumors[2].GetSize() * fontSize), startHeight + 570 };
	};

	void Move(float x, float y = 0)
	{
		name.SetPosition(name.position.x + x, name.position.y);
		age.SetPosition(age.position.x + x, age.position.y);
		height.SetPosition(height.position.x + x, height.position.y);
		shoesize.SetPosition(shoesize.position.x + x, shoesize.position.y);

		img.SetPosition(img.position.x + x, img.position.y);
		profile.SetPosition(profile.position.x + x, profile.position.y);

		characteristicsHeading.SetPosition(characteristicsHeading.position.x + x, characteristicsHeading.position.y);
		characteristics[0].SetPosition(characteristics[0].position.x + x, characteristics[0].position.y);
		characteristics[1].SetPosition(characteristics[1].position.x + x, characteristics[1].position.y);
		characteristics[2].SetPosition(characteristics[2].position.x + x, characteristics[2].position.y);

		rumorsHeading.SetPosition(rumorsHeading.position.x + x, rumorsHeading.position.y);

		rumors[0].SetPosition(rumors[0].position.x + x, rumors[0].position.y);
		rumors[1].SetPosition(rumors[1].position.x + x, rumors[1].position.y);
		rumors[2].SetPosition(rumors[2].position.x + x, rumors[2].position.y);
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush)
	{
		img.Draw();
		if (profile.visible)
			profile.Draw();

		name.Draw(format, brush);
		age.Draw(format, brush);
		height.Draw(format, brush);
		shoesize.Draw(format, brush);
		characteristicsHeading.Draw(format, brush);
		characteristics[0].Draw(format, brush);
		characteristics[1].Draw(format, brush);
		characteristics[2].Draw(format, brush);
		rumorsHeading.Draw(format, brush);
		rumors[0].Draw(format, brush);
		rumors[1].Draw(format, brush);
		rumors[2].Draw(format, brush);
	}
};

struct Slot
{
	D2D_VECTOR_2F position = {};
	unsigned int ID = -1;
	Button button;
	Image image;
	Image profile;
	Text name;

	Slot() = default;
	Slot(std::wstring text, LPCWSTR path, D2D_VECTOR_2F position, unsigned int ID)
		:position(position), ID(ID)
	{
		image = Image(path, 1.0f, true, position);
		profile = Image(L"./UI/Unknown.png", 1.0f, false, image.position);
		name = Text(text, true, position, image.width + 50);
		button = Button(true, position, image.width, image.height + 40);
	}

	void SetPosition(float x, float y)
	{
		position = { x, y };
		image.SetPosition(x, y);
		name.SetPosition(x, y + 80);
		profile.SetPosition(x, y);
		button.SetPosition(x, y + 20);
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush)
	{
		image.Draw();
		if (profile.visible)
			profile.Draw();

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

	double slideSpeed = 90.0f;

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

	int GetIDFromName(std::string name)
	{
		std::wstring wName(name.begin(), name.end());

		for (auto& page : pages)
		{
			if (page.name.GetText() == wName)
				return page.ID;
		}

		return -1;
	}
public:
	Journal()
	{
		Event::Bind(this, EventType::LEFTCLICK);

		journalBase = Image(L"./UI/Journal.png", 1.0f, false, { (float)Window::GetWidth() + 360, (float)Window::GetHeight() / 2 });

		exitCross = Image(L"./UI/ExitCross.png", 1.0f, true, { journalBase.position.x - 240, journalBase.position.y - 340});
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
			slots[i] = Slot(L"Unknown", L"./UI/Unknown.png", slotPositions[i], i);

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
		for (unsigned int i = 0; i < 9; ++i)
		{
			slots[i] = Slot(L"Unknown", L"./UI/Unknown.png", slotPositions[i], i);
			pages[i] = Page();
		}
	}

	void Draw(IDWriteTextFormat& format, ID2D1Brush& brush, float dt)
	{
		if (slideIn)
		{
			float distance = maxX - journalBase.position.x;
			float amount = slideSpeed / ((distance + 100.0f) * 0.0002f) * dt;
			journalBase.SetPosition(journalBase.position.x - amount, journalBase.position.y);
			exitCross.SetPosition(exitCross.position.x - amount, exitCross.position.y);
			exitButton.SetPosition(exitButton.position.x - amount, exitButton.position.y);

			for (auto& page : pages)
				page.Move(-amount);

			for (auto& slot : slots)
				slot.SetPosition(slot.position.x - amount, slot.position.y);

			if (journalBase.position.x <= minX)
			{
				fullyShowing = true;
				slideIn = false;
			}
		}

		if (slideOut)
		{
			float distance = maxX - journalBase.position.x;
			float amount = slideSpeed / ((distance + 100.0f) * 0.0002f) * dt;
			journalBase.SetPosition(journalBase.position.x + amount, journalBase.position.y);
			exitCross.SetPosition(exitCross.position.x + amount, exitCross.position.y);
			exitButton.SetPosition(exitButton.position.x + amount, exitButton.position.y);
			
			for (auto& page : pages)
				page.Move(amount);

			for (auto& slot : slots)
				slot.SetPosition(slot.position.x + amount, slot.position.y);

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

	void AddSuspect(unsigned int ID, std::string name, std::string characteristics1 = "", std::string characteristics2 = "", std::string characteristics3 = "", int age = 0, int height = 0, int shoeSize = 0)
	{
		std::wstring temp(name.begin(), name.end());

		slots[ID].ID = ID;
		slots[ID].name = Text(temp, true, slots[ID].name.position, 500);

		pages[ID].ID = ID;
		pages[ID].name = Text(L"Name: " + temp, true, pages[ID].name.position, 500);

		//IMAGE
		if (age != 0)
		{
			LPCWSTR imgName = L"";

			if (name == "Fabian Voltaire")
				imgName = L"./UI/Fabian.png";

			if (name == "Franklin Pickett")
				imgName = L"./UI/Franklin.png";

			if (name == "Sally Bennett")
				imgName = L"./UI/Sally.png";

			if (name == "Mayor A. Rock")
				imgName = L"./UI/Mayor.png";

			if (name == "Timothy Cooke")
				imgName = L"./UI/Timothy.png";

			if (name == "Mr. Wicker")
				imgName = L"./UI/Wicker.png";

			if (name == "Olga Morozov")
				imgName = L"./UI/Olga.png";

			if (name == "Claudette Tasse")
				imgName = L"./UI/Claudette.png";

			if (name == "Tess Singer")
				imgName = L"./UI/Tess.png";

			slots[ID].profile = Image(imgName, 1.2f, true, slots[ID].profile.position);
			pages[ID].profile = Image(imgName, 1.55f, true, pages[ID].profile.position);
		}

		//AGE
		if (age != 0)
			pages[ID].age = Text(L"Age: " + std::to_wstring(age), true, pages[ID].age.position, 500);
		else
			pages[ID].age = Text(L"Age: XX", true, pages[ID].age.position, 500);

		//HEIGHT
		if (height != 0)
			pages[ID].height = Text(L"Height: " + std::to_wstring(height) + L" cm", true, pages[ID].height.position, 500);
		else
			pages[ID].height = Text(L"Height: XXX cm", true, pages[ID].height.position, 500);

		//SHOE SIZE
		if (shoeSize != 0)
			pages[ID].shoesize = Text(L"Shoesize: " + std::to_wstring(shoeSize), true, pages[ID].shoesize.position, 500);
		else
			pages[ID].shoesize = Text(L"Shoesize: XX", true, pages[ID].shoesize.position, 500);

		//CHARACTERISTICS
		if (characteristics1 != "" || characteristics2 != "" || characteristics3 != "")
			pages[ID].characteristicsHeading = Text(L"Characteristics:", true, pages[ID].characteristicsHeading.position, 500);

		std::wstring char1(characteristics1.begin(), characteristics1.end());
		if (characteristics1 != "")
			pages[ID].characteristics[0] = Text(char1, true, pages[ID].characteristics[0].position, 500);

		std::wstring char2(characteristics2.begin(), characteristics2.end());
		if (characteristics2 != "")
			pages[ID].characteristics[1] = Text(char2, true, pages[ID].characteristics[1].position, 500);

		std::wstring char3(characteristics3.begin(), characteristics3.end());
		if (characteristics3 != "")
			pages[ID].characteristics[2] = Text(char3, true, pages[ID].characteristics[2].position, 500);
	}

	void AddRumour(std::string name, std::string rumour)
	{
		std::wstring newRumour(rumour.begin(), rumour.end());
		std::wstring pageName(name.begin(), name.end());
		pageName = L"Name: " + pageName;

		for (auto& page : pages)
		{
			if (page.name.GetText() == pageName)
			{
				if (page.rumorsHeading.GetText() == L"")
					page.rumorsHeading = Text(L"Rumours:", true, page.rumorsHeading.position, 500);

				for (int i = 0; i < 3; ++i)
				{
					if (page.rumors[i].GetText() == newRumour)
						break; 

					if (page.rumors[i].GetText() == L"")
					{
						page.rumors[i] = Text(newRumour, true, { page.rumors[i].position.x + 100, page.rumors[i].position.y }, 700);
						break;
					}
				}			
			}	
		}	
	}

	void UpdateSuspect(std::string name, std::string characteristics1, std::string characteristics2, std::string characteristics3, int age, int height, int shoeSize)
	{
		std::wstring temp(name.begin(), name.end());
		int ID = GetIDFromName("Name: " + name);
		if (ID < 0)
			return;

		//AGE
		pages[ID].age = Text(L"Age: " + std::to_wstring(age), true, pages[ID].age.position, 500);

		//HEIGHT
		pages[ID].height = Text(L"Height: " + std::to_wstring(height) + L" cm", true, pages[ID].height.position, 500);

		//SHOE SIZE
		pages[ID].shoesize = Text(L"Shoesize: " + std::to_wstring(shoeSize), true, pages[ID].shoesize.position, 500);

		//CHARACTERISTICS
		pages[ID].characteristicsHeading = Text(L"Characteristics:", true, pages[ID].characteristicsHeading.position, 500);

		std::wstring char1(characteristics1.begin(), characteristics1.end());
		pages[ID].characteristics[0] = Text(char1, true, pages[ID].characteristics[0].position, 500);

		std::wstring char2(characteristics2.begin(), characteristics2.end());
		pages[ID].characteristics[1] = Text(char2, true, pages[ID].characteristics[1].position, 500);

		std::wstring char3(characteristics3.begin(), characteristics3.end());
		pages[ID].characteristics[2] = Text(char3, true, pages[ID].characteristics[2].position, 500);

		//IMAGE
		LPCWSTR imgName = L"";

		if (name == "Fabian Voltaire")
			imgName = L"./UI/Fabian.png";

		if (name == "Franklin Pickett")
			imgName = L"./UI/Franklin.png";

		if (name == "Sally Bennett")
			imgName = L"./UI/Sally.png";

		if (name == "Mayor A. Rock")
			imgName = L"./UI/Mayor.png";

		if (name == "Timothy Cooke")
			imgName = L"./UI/Timothy.png";

		if (name == "Mr. Wicker")
			imgName = L"./UI/Wicker.png";

		if (name == "Olga Morozov")
			imgName = L"./UI/Olga.png";

		if (name == "Claudette Tasse")
			imgName = L"./UI/Claudette.png";

		if (name == "Tess Singer")
			imgName = L"./UI/Tess.png";

		slots[ID].profile = Image(imgName, 1.2f, true, slots[ID].profile.position);
		pages[ID].profile = Image(imgName, 1.55f, true, pages[ID].profile.position);
	}

	bool HasSuspect(std::string name)
	{
		std::wstring temp(name.begin(), name.end());

		for (int i = 0; i < 9; ++i)
			if (temp == slots[i].name.GetText())
				return true;

		return false;
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