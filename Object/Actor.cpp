#include "pch.h"
#include "Actor.h"
#include "Texture.h"

Actor::~Actor()
{
}

void Actor::Init()
{
}

void Actor::Update()
{
}

void Actor::Render(HDC hdc)
{
	int32 sizeX = _texture->GetSize().x;
	int32 sizeY = _texture->GetSize().y;

	/*::TransparentBlt(hdc,
		_pos.x - sizeX / 2,
		_pos.y - sizeY / 2,
		sizeX, sizeY,
		_texture->GetDC(),
		0,
		0,
		sizeX, sizeY,
		_texture->GetTransparent());*/
	
	BitBlt(hdc, _pos.x - sizeX / 2, _pos.y - sizeY / 2, sizeX, sizeY,
		_texture->GetDC(), 0, 0, SRCCOPY);
}
