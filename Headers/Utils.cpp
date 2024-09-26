#include "Utils.h"
#include "pch.h"
#include "Piece.h"

HFONT Utils::_font = ::CreateFont(
    15,                              // 폰트 높이 (크기)
    0,                              // 폭 (0으로 설정하여 기본값 사용)
    0,                              // 각도
    0,                              // 문자베이스 각도
    FW_SEMIBOLD,                      // 굵기 (FW_BOLD 사용 가능)
    FALSE,                          // 기울임꼴 여부
    FALSE,                          // 밑줄 여부
    FALSE,                          // 취소선 여부
    DEFAULT_CHARSET,                // 문자셋
    OUT_DEFAULT_PRECIS,             // 출력 정확도
    CLIP_DEFAULT_PRECIS,            // 클리핑 정확도
    DEFAULT_QUALITY,                // 출력 품질
    DEFAULT_PITCH | FF_SWISS,       // 피치 및 패밀리
    L"나눔명조"                // 폰트 이름
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
        fontSize,                              // 폰트 높이 (크기)
        0,                              // 폭 (0으로 설정하여 기본값 사용)
        0,                              // 각도
        0,                              // 문자베이스 각도
        FW_BOLD,                      // 굵기 (FW_BOLD 사용 가능)
        FALSE,                          // 기울임꼴 여부
        FALSE,                          // 밑줄 여부
        FALSE,                          // 취소선 여부
        DEFAULT_CHARSET,                // 문자셋
        OUT_DEFAULT_PRECIS,             // 출력 정확도
        CLIP_DEFAULT_PRECIS,            // 클리핑 정확도
        DEFAULT_QUALITY,                // 출력 품질
        DEFAULT_PITCH | FF_SWISS,       // 피치 및 패밀리
        L"나눔명조"                // 폰트 이름
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

    // 원 그리기
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


