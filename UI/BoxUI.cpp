#include "pch.h"
#include "BoxUI.h"
#include "InputManager.h"
#include "Board.h"

BoxUI::BoxUI(const VectorInt leftTop, const VectorInt rightBottom, const VectorInt textPos, 
	const wstring& text, COLORREF color)
	: Super(UI_Square)
	, _color(color), _text(text), _leftTop(leftTop), _rightBottom(rightBottom), _textPos(textPos)
{
}

void BoxUI::Init()
{
	
	
}

void BoxUI::Update()
{
	VectorInt mousePos = GET_SINGLE(InputManager)->GetMousePos();
	_isMouseOnBox = (mousePos >= _leftTop && mousePos <= _rightBottom) ? true : false;

	if (_isMouseOnBox)
	{
		if (GET_SINGLE(InputManager)->IsKeyDown(LClick))
			Execute();
	}
}

void BoxUI::Render(HDC hdc)
{
	Utils::DrawRectangle(hdc, _leftTop, _rightBottom, _isMouseOnBox);
	Utils::WriteText(hdc, _textPos, _text);
}
