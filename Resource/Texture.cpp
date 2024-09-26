
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

    // ��� ���ڿ� ��ȯ (std::string -> std::wstring)
    wstring wPath(path.begin(), path.end());

    // �̹��� �ε�
    Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(wPath.c_str());
    if (!image || image->GetLastStatus() != Ok) {
        
        GdiplusShutdown(gdiplusToken);
        return nullptr;
    }

    // �̹��� ũ�� ��������
    _size.x = image->GetWidth();
    _size.y = image->GetHeight();

    // HDC ����
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

    // GDI+���� GDI�� ��Ʈ���� ����
    {
        Graphics graphics(_hdc);
        graphics.DrawImage(image, 0, 0, _size.x, _size.y);
    }



    // GDI+ ����
    delete image;
    GdiplusShutdown(gdiplusToken);

    return this;
}
