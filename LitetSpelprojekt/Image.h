#pragma once
#include <wincodec.h>
#include "UIComponent.h"

class Image :public UIComponent
{
private:
	float scale = 1.0f;
	ComPtr<ID2D1Bitmap> bitmap;
	float opacity = 1.0f;
public:
	Image() = default;

	Image(LPCWSTR filename, float scale, bool visible, D2D_VECTOR_2F position)
		:UIComponent(visible, position), scale(scale)
	{
		HRESULT hr;

		ComPtr<IWICImagingFactory> wicFactory = nullptr;
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);

		if SUCCEEDED(hr)
		{
			ComPtr<IWICBitmapDecoder> wicDecoder;
			hr = wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);

			if SUCCEEDED(hr)
			{
				ComPtr<IWICBitmapFrameDecode> wicFrame;
				hr = wicDecoder->GetFrame(0, &wicFrame);
				if SUCCEEDED(hr)
				{
					ComPtr<IWICFormatConverter> wicConverter;
					hr = wicFactory->CreateFormatConverter(&wicConverter);
					if SUCCEEDED(hr)
					{
						hr = wicConverter->Initialize(wicFrame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
						if SUCCEEDED(hr)
						{
							hr = Graphics::Get2DRenderTarget().CreateBitmapFromWicBitmap(wicConverter.Get(), &bitmap);
							if FAILED(hr)
								Error("FAILED TO CREATE BITMAP");
						}
						else
							Error("FAILED TO INITIALIZE WIC CONVERTER");
					}
					else
						Error("FAILED TO CREATE FORMAT CONVERTER");
				}
				else
					Error("FAILED TO GET IMAGE");
			}
			else
				Error("FAILED TO CREATE WIC DECODER");
		}
		else
			Error("FAILED TO CREATE WIC FACTORY");

		this->scale = scale;
		this->width = scale * bitmap->GetSize().width;
		this->height = scale * bitmap->GetSize().height;

		bounds = {
				position.x - width / 2,
				position.y - height / 2,
				position.x + width / 2,
				position.y + height / 2
				};
	}

	void SetScale(float value)
	{
		this->width = value * bitmap->GetSize().width;
		this->height = value * bitmap->GetSize().height;

		bounds = {
				position.x - width / 2,
				position.y - height / 2,
				position.x + width / 2,
				position.y + height / 2
		};
	}

	void ResetScale()
	{
		this->width = scale * bitmap->GetSize().width;
		this->height = scale * bitmap->GetSize().height;

		bounds = {
				position.x - width / 2,
				position.y - height / 2,
				position.x + width / 2,
				position.y + height / 2
		};
	}

	void SetOpacity(float opacity)
	{
		this->opacity = opacity;
	}

	float GetOpacity() const
	{
		return this->opacity;
	}

	void ReduceOpacity(float amount)
	{
		this->opacity -= amount;
	}

	void Draw()
	{
		Graphics::Get2DRenderTarget().DrawBitmap(bitmap.Get(), bounds, opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0.0f, 0.0f, bitmap->GetSize().width, bitmap->GetSize().height));
	}
};