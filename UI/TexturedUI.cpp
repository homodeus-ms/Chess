#include "pch.h"
#include "TexturedUI.h"

TexturedUI::TexturedUI(VectorInt pos, Texture* texture)
	: Super(UI_Texture)
{
	SetTexture(texture);
}

void TexturedUI::Init()
{
}

void TexturedUI::Update()
{
}

void TexturedUI::Render(HDC hdc)
{
	Super::Render(hdc);
}
