#pragma once

extern HDC G_HDC;

class Game
{
public:
	Game() {}
	~Game() {}

	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	HWND _hwnd;
	HDC _hdc;

	// Double Buffering
	HDC _hdcBack = {};
	RECT _rect = {};
	HBITMAP _bmpBack = {};
};

