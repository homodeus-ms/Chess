#pragma once
#include "UI.h"
class TexturedUI : public UI
{
	using Super = UI;
public:
	TexturedUI(VectorInt pos, Texture* texture);
	virtual ~TexturedUI() {};

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	
};

