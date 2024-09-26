#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

HDC G_HDC = nullptr;

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_hdc = ::GetDC(_hwnd);

	::GetClientRect(_hwnd, &_rect);
	_hdcBack = ::CreateCompatibleDC(_hdc);

	G_HDC = _hdcBack;

	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom);
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack);
	::DeleteObject(prev);

	Utils::Init(_hdcBack);
	GET_SINGLE(InputManager)->Init(_hwnd);
	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(ResourceManager)->Init(_hwnd, "C:\\kiwi\\Chess\\Chess\\Resources\\");
	GET_SINGLE(SceneManager)->Init();

}

void Game::Update()
{
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(SceneManager)->Update();
}

void Game::Render()
{
	GET_SINGLE(SceneManager)->Render(_hdcBack);
	GET_SINGLE(TimeManager)->Render(_hdcBack);
	GET_SINGLE(InputManager)->Render(_hdcBack);
	// Double Buffering
	::BitBlt(_hdc, 0, 0, G_WINSIZE_X, G_WINSIZE_Y, _hdcBack, 0, 0, SRCCOPY);
	::PatBlt(_hdcBack, 0, 0, G_WINSIZE_X, G_WINSIZE_Y, WHITENESS);
}
