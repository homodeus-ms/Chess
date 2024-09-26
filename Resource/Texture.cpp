
#include "pch.h"
#include "Texture.h"


#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;


Texture* Texture::LoadFromFile(HWND hwnd, const string& path)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // 경로 문자열 변환 (std::string -> std::wstring)
    wstring wPath(path.begin(), path.end());

    // 이미지 로드
    Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(wPath.c_str());
    if (!image || image->GetLastStatus() != Ok) {
        
        GdiplusShutdown(gdiplusToken);
        return nullptr;
    }

    // 이미지 크기 가져오기
    _size.x = image->GetWidth();
    _size.y = image->GetHeight();

    // HDC 생성
    HDC hdc = ::GetDC(hwnd);
    _hdc = ::CreateCompatibleDC(hdc);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = _size.x;
    bmi.bmiHeader.biHeight = -_size.y; // Top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* bits = nullptr;
    _bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);

    if (!_bitmap)
    {
        delete image;
        ::ReleaseDC(hwnd, hdc);
        Gdiplus::GdiplusShutdown(gdiplusToken);
        
        return nullptr;
    }

    HGDIOBJ prev = SelectObject(_hdc, _bitmap);
    ::DeleteObject(prev);

    // GDI+에서 GDI로 비트맵을 복사
    {
        Graphics graphics(_hdc);
        graphics.DrawImage(image, 0, 0, _size.x, _size.y);
    }



    // GDI+ 해제
    delete image;
    GdiplusShutdown(gdiplusToken);

    return this;
}
