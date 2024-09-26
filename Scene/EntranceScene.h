#pragma once
#include "GameScene.h"

class EntranceScene : public GameScene
{
	using Super = GameScene;
public:
	EntranceScene();
	virtual ~EntranceScene();

	void Init() override;
	void Update() override;
	void Render(HDC hdc) override;

private:
	void DrawSelectBlackCircle(HDC hdc);
	void DrawSelectWhiteCircle(HDC hdc);
	void DrawSelectEasy(HDC hdc);
	void DrawSelectNormal(HDC hdc);
	void DrawSelectHard(HDC hdc);

private:
	
	const VectorInt TITLE_POS = { 70, 65 };
	const VectorInt SELECT_STR_POS = { 155, 390 };

	const VectorInt SELECT_BLACK_POS = { 120, 480 };
	const VectorInt SELECT_WHITE_POS = { 470, 480 };

	const VectorInt SELECT_LEVEL_EASY_POS = { 920, 400 };
	const VectorInt SELECT_LEVEL_NORMAL_POS = { 920, 480 };
	const VectorInt SELECT_LEVEL_HARD_POS = { 920, 560 };

	const uint32 TITLE_FONT_SIZE = 240;
	const uint32 COLOR_FONT_SIZE = 100;
	const uint32 LEVEL_FONT_SIZE = 60;
	const uint32 SELECT_STR_FONT_SIZE = 50;
	
	const COLORREF MOUSE_ON_COLOR = RGB(150, 150, 150);
	const COLORREF SELECTED_COLOR = RGB(220, 120, 120);
	
	enum MouseOnFlag : uint8
	{
		SelectBlack,
		SelectWhite,
		SelectEasy,
		SelectNormal,
		SelectHard,
	};

	uint8 _mouseOnFlag = 0;
	bool _colorSelected = false;
	bool _levelSelected = false;

};

