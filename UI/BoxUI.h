#pragma once
#include "UI.h"
class BoxUI : public UI
{
	using Super = UI;
public:
	BoxUI(const VectorInt leftTop, const VectorInt rightBottom, VectorInt textPos, const wstring& text, COLORREF color = RGB(255, 255, 255));
	virtual ~BoxUI() {};

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	const VectorInt _leftTop;
	const VectorInt _rightBottom;
	const wstring _text;
	const VectorInt _textPos;
	COLORREF _color;
	
};

