#pragma once
#include <wincodec.h>
#include "UIComponent.h"

class Image :public UIComponent
{
private:
	float scale;
	ComPtr<ID2D1Bitmap> bitmap;
public:
	Image(LPCWSTR filename, ID2D1RenderTarget& renderTarget, float scale, bool visible, D2D_VECTOR_2F position)
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

	void Draw(ID2D1RenderTarget& renderTarget)
	{
		renderTarget.DrawBitmap(bitmap.Get(), bounds, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0.0f, 0.0f, bitmap->GetSize().width, bitmap->GetSize().height));
	}
};