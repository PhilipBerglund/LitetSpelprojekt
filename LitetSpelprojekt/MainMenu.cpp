#include "MainMenu.h"

MainMenu::MainMenu()
	:image(nullptr)
{
}

bool MainMenu::Initialize(ID2D1Factory& factory, ID2D1RenderTarget& renderTarget)
{
	if FAILED(renderTarget.CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightCoral), &brush))
		return false;

	image = new Image(L"UI/transparency.png", renderTarget, 0.3f, renderTarget.GetSize().width / 2, renderTarget.GetSize().height / 2);
	buttons.push_back(new Button(factory, 40, 40, 40, 40));
	visibleComponents.push_back(buttons[0]);

	return true;
}

void MainMenu::Render(ID2D1Factory& factory, ID2D1RenderTarget& renderTarget)
{
	renderTarget.BeginDraw();

	//???
	renderTarget.SetTransform(D2D1::Matrix3x2F::Identity());

	for (int i = 0; i < visibleComponents.size(); ++i)
		visibleComponents[i]->Draw(renderTarget, brush.Get());

	image->Draw(renderTarget);
	renderTarget.EndDraw();
}

void MainMenu::GetInput(int x, int y)
{
	for (int i = 0; i < visibleComponents.size(); ++i)
	{
		if (visibleComponents[i]->OnClick(x, y))
		{
			Print("CLICKED ON BUTTON");
		}

		//image->MoveTo(x, y);
	}
}