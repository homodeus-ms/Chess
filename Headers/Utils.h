#pragma once
#include "windows.h"
#include <string>
#include "Types.h"
//#include "Piece.h"

class Piece;

class Utils
{
public:
	static void Init(HDC hdc);

	static void WriteText(HDC hdc, const VectorInt pos, const std::wstring& text);
	static void WriteText(HDC hdc, const VectorInt pos, const std::wstring& text, uint32 fontSize, COLORREF color = RGB(0, 0, 0));
	static void DrawCircle(HDC hdc, const VectorInt pos, float radius, COLORREF color);
	static void DrawRectangle(HDC hdc, const VectorInt& lTop, const VectorInt& rBottom, bool colored = false);

	static HFONT _font;
	static HBRUSH _blueBrush;
};