#include "Utils.h"
#include "pch.h"
#include "Piece.h"

HFONT Utils::_font = ::CreateFont(
    15,                              // ��Ʈ ���� (ũ��)
    0,                              // �� (0���� �����Ͽ� �⺻�� ���)
    0,                              // ����
    0,                              // ���ں��̽� ����
    FW_SEMIBOLD,                      // ���� (FW_BOLD ��� ����)
    FALSE,                          // ����Ӳ� ����
    FALSE,                          // ���� ����
    FALSE,                          // ��Ҽ� ����
    DEFAULT_CHARSET,                // ���ڼ�
    OUT_DEFAULT_PRECIS,             // ��� ��Ȯ��
    CLIP_DEFAULT_PRECIS,            // Ŭ���� ��Ȯ��
    DEFAULT_QUALITY,                // ��� ǰ��
    DEFAULT_PITCH | FF_SWISS,       // ��ġ �� �йи�
    L"��������"                // ��Ʈ �̸�
);

HBRUSH Utils::_blueBrush = ::CreateSolidBrush(RGB(88, 148, 220));

void Utils::Init(HDC hdc)
{
    HFONT oldOne = static_cast<HFONT>(::SelectObject(hdc, _font));
    ::DeleteObject(oldOne);
    ::SetBkMode(hdc, TRANSPARENT);
}

void Utils::WriteText(HDC hdc, const VectorInt pos, const std::wstring& text)
{
	::TextOut(hdc, pos.x, pos.y, text.c_str(), static_cast<int>(text.size()));
}

void Utils::WriteText(HDC hdc, const VectorInt pos, const std::wstring& text, uint32 fontSize, COLORREF color)
{
    //HPEN redPen = ::CreatePen(PS_SOLID, fontSize, RGB(220, 30, 30));
    //auto oldPen = ::SelectObject(hdc, redPen);

    HFONT font = ::CreateFont(
        fontSize,                              // ��Ʈ ���� (ũ��)
        0,                              // �� (0���� �����Ͽ� �⺻�� ���)
        0,                              // ����
        0,                              // ���ں��̽� ����
        FW_BOLD,                      // ���� (FW_BOLD ��� ����)
        FALSE,                          // ����Ӳ� ����
        FALSE,                          // ���� ����
        FALSE,                          // ��Ҽ� ����
        DEFAULT_CHARSET,                // ���ڼ�
        OUT_DEFAULT_PRECIS,             // ��� ��Ȯ��
        CLIP_DEFAULT_PRECIS,            // Ŭ���� ��Ȯ��
        DEFAULT_QUALITY,                // ��� ǰ��
        DEFAULT_PITCH | FF_SWISS,       // ��ġ �� �йи�
        L"��������"                // ��Ʈ �̸�
    );

    
    ::SetTextColor(hdc, color);
   
    HFONT oldOne = static_cast<HFONT>(::SelectObject(hdc, font));
    ::TextOut(hdc, pos.x, pos.y, text.c_str(), static_cast<int>(text.size()));
    ::SelectObject(hdc, oldOne);
    ::DeleteObject(font);

    ::SetTextColor(hdc, RGB(0, 0, 0));

    /*::SelectObject(hdc, oldPen);
    ::DeleteObject(redPen);*/

}

void Utils::DrawCircle(HDC hdc, const VectorInt pos, float radius, COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // �� �׸���
    Ellipse(hdc,
        static_cast<int32_t>(pos.x - radius),
        static_cast<int32_t>(pos.y - radius),
        static_cast<int32_t>(pos.x + radius),
        static_cast<int32_t>(pos.y + radius));
    
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

void Utils::DrawRectangle(HDC hdc, const VectorInt& leftTop, const VectorInt& rightBottom, bool colored)
{
    if (colored)
    {
        HBRUSH oldB = static_cast<HBRUSH>(::SelectObject(hdc, _blueBrush));
        ::Rectangle(hdc, leftTop.x, leftTop.y, rightBottom.x, rightBottom.y);
        ::SelectObject(hdc, oldB);
    }
    else
        ::Rectangle(hdc, leftTop.x, leftTop.y, rightBottom.x, rightBottom.y);

}


