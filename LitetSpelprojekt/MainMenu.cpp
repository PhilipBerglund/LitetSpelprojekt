#include "MainMenu.h"

MainMenu::MainMenu()
{
}

bool MainMenu::Initialize(ID2D1RenderTarget& renderTarget)
{
	if (FAILED(renderTarget.CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightCoral), &brush)))
		return false;

	rectangle = D2D1::RectF(renderTarget.GetSize().width / 2 - 50.0f,
		renderTarget.GetSize().height / 2 - 50.0f,
		renderTarget.GetSize().width / 2 + 50.0f,
		renderTarget.GetSize().height / 2 + 50.0f);

	renderTarget.FillRectangle(&rectangle, brush.Get());
	return true;
}

void MainMenu::Render(ID2D1RenderTarget& renderTarget)
{
	renderTarget.BeginDraw();
	renderTarget.SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget.Clear();

	renderTarget.FillRectangle(&rectangle, brush.Get());
	renderTarget.EndDraw();
}
