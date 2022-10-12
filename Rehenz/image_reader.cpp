#include "image_reader.h"
#define WIN32_LEAN_AND_MEAN
#include <wrl/client.h>
#include <wincodec.h>

namespace Rehenz
{
    using Microsoft::WRL::ComPtr;
    class CCoInitialize
    {
    public:
        HRESULT hr;
        CCoInitialize() { hr = CoInitialize(nullptr); }
        ~CCoInitialize() { if (SUCCEEDED(hr)) CoUninitialize(); }
    };
    std::unique_ptr<uint[]> LoadImageFile(const std::wstring& filename, uint& width, uint& height)
    {
        HRESULT hr = S_OK;

        CCoInitialize init;
        if (FAILED(init.hr))
            return nullptr;

        // create wic factory
        ComPtr<IWICImagingFactory2> wic_factory;
        hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(wic_factory.GetAddressOf()));
        if (FAILED(hr))
            return nullptr;

        // load image from file
        ComPtr<IWICBitmapDecoder> bitmap_src;
        ComPtr<IWICBitmapFrameDecode> frame;
        hr = wic_factory->CreateDecoderFromFilename(filename.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, bitmap_src.GetAddressOf());
        if (FAILED(hr))
            return nullptr;
        hr = bitmap_src->GetFrame(0, frame.GetAddressOf());
        if (FAILED(hr))
            return nullptr;

        // convert file format
        // https://learn.microsoft.com/en-us/windows/win32/wic/-wic-codec-native-pixel-formats
        ComPtr<IWICFormatConverter> format_converter;
        WICPixelFormatGUID wic_format_in;
        WICPixelFormatGUID wic_format_out = GUID_WICPixelFormat32bppBGRA;
        BOOL can_convert;
        hr = frame->GetPixelFormat(&wic_format_in);
        if (FAILED(hr))
            return nullptr;
        hr = wic_factory->CreateFormatConverter(format_converter.GetAddressOf());
        if (FAILED(hr))
            return nullptr;
        hr = format_converter->CanConvert(wic_format_in, wic_format_out, &can_convert);
        if (FAILED(hr) || !can_convert)
            return nullptr;
        hr = format_converter->Initialize(frame.Get(), wic_format_out, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);
        if (FAILED(hr))
            return nullptr;

        // load info
        hr = format_converter->GetSize(&width, &height);
        if (FAILED(hr))
            return nullptr;

        // create array
        uint size = width * height;
        std::unique_ptr<uint[]> image = std::make_unique<uint[]>(size);

        // load pixels
        hr = format_converter->CopyPixels(nullptr, width * sizeof(uint), size * sizeof(uint), reinterpret_cast<BYTE*>(image.get()));
        if (FAILED(hr))
            return nullptr;

        return image;
    }
}