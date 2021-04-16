#pragma once
#include <wincodec.h>
#include "UI_Component.h"


class Image
{
private:
	float uniformScale;
	ComPtr<ID2D1Bitmap> bitmap;
	D2D1_RECT_F rect = {};
public:
	Image(LPCWSTR filename, ID2D1RenderTarget& renderTarget, float scale, float x, float y)
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
							hr = renderTarget.CreateBitmapFromWicBitmap(wicConverter.Get(), &bitmap);
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

		this->uniformScale = scale;

		rect = {
				x - uniformScale * bitmap->GetSize().width / 2,
				y - uniformScale * bitmap->GetSize().height / 2,
				x + uniformScale * bitmap->GetSize().width / 2,
				y + uniformScale * bitmap->GetSize().height / 2
				};
	}

	void Draw(ID2D1RenderTarget& renderTarget)
	{
		renderTarget.DrawBitmap(bitmap.Get(), rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0.0f, 0.0f, bitmap->GetSize().width, bitmap->GetSize().height));
	}

	void MoveTo(float x, float y)
	{
		rect = {
				x - uniformScale * bitmap->GetSize().width / 2,
				y - uniformScale * bitmap->GetSize().height / 2,
				x + uniformScale * bitmap->GetSize().width / 2,
				y + uniformScale * bitmap->GetSize().height / 2
		};
	}
};