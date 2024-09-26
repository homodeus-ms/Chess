#include "pch.h"
#include "EntranceScene.h"
#include "InputManager.h"
#include "SceneManager.h"

EntranceScene::EntranceScene()
	: Super(SceneType::EntranceScene)
{
}

EntranceScene::~EntranceScene()
{
}

void EntranceScene::Init()
{
}

void EntranceScene::Update()
{
	if (_colorSelected && _levelSelected)
	{
		Sleep(300);
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::InfoScene);
		return;
	}

	InputManager* inputManager = GET_SINGLE(InputManager);
	const VectorInt diff(140, 50);

	if (inputManager->IsCursorInBox(SELECT_BLACK_POS, SELECT_BLACK_POS + VectorInt(250, 100)))
	{
		_mouseOnFlag |= 1 << SelectBlack;
		if (inputManager->IsKeyDown(LClick))
		{
			_colorSelected = true;
			s_playerColor = ColorType::Black;
		}
	}
	else if (inputManager->IsCursorInBox(SELECT_WHITE_POS, SELECT_WHITE_POS + VectorInt(250, 100)))
	{
		_mouseOnFlag |= 1 << SelectWhite;
		if (inputManager->IsKeyDown(LClick))
		{
			_colorSelected = true;
			s_playerColor = ColorType::White;
		}
	}
	else if (inputManager->IsCursorInBox(SELECT_LEVEL_EASY_POS, SELECT_LEVEL_EASY_POS + diff))
	{
		_mouseOnFlag |= 1 << SelectEasy;
		if (inputManager->IsKeyDown(LClick))
		{
			_levelSelected = true;
			s_gameLevel = LEVEL_EASY;
		}
	}
	else if (inputManager->IsCursorInBox(SELECT_LEVEL_NORMAL_POS, SELECT_LEVEL_NORMAL_POS + VectorInt(220, 50)))
	{
		_mouseOnFlag |= 1 << SelectNormal;
		if (inputManager->IsKeyDown(LClick))
		{
			_levelSelected = true;
			s_gameLevel = LEVEL_NORMAL;
		}
	}
	else if (inputManager->IsCursorInBox(SELECT_LEVEL_HARD_POS, SELECT_LEVEL_HARD_POS + diff))
	{
		_mouseOnFlag |= 1 << SelectHard;
		if (inputManager->IsKeyDown(LClick))
		{
			_levelSelected = true;
			s_gameLevel = LEVEL_HARD;
		}
	}


}

void EntranceScene::Render(HDC hdc)
{
	Utils::WriteText(hdc, TITLE_POS, L"CHESS", TITLE_FONT_SIZE);

	DrawSelectBlackCircle(hdc);
	DrawSelectWhiteCircle(hdc);
	DrawSelectEasy(hdc);
	DrawSelectNormal(hdc);
	DrawSelectHard(hdc);

	_mouseOnFlag = 0;
}

void EntranceScene::DrawSelectBlackCircle(HDC hdc)
{
	COLORREF color = _mouseOnFlag & (1 << SelectBlack) ? MOUSE_ON_COLOR : RGB(0, 0, 0);
	color = _colorSelected && s_playerColor == ColorType::Black ? SELECTED_COLOR : color;
	Utils::WriteText(hdc, SELECT_BLACK_POS, L"BLACK", COLOR_FONT_SIZE, color);
}

void EntranceScene::DrawSelectWhiteCircle(HDC hdc)
{
	COLORREF color = _mouseOnFlag & (1 << SelectWhite) ? MOUSE_ON_COLOR : RGB(0, 0, 0);
	color = _colorSelected && s_playerColor == ColorType::White ? SELECTED_COLOR : color;
	Utils::WriteText(hdc, SELECT_WHITE_POS, L"WHITE", COLOR_FONT_SIZE, color);
}

void EntranceScene::DrawSelectEasy(HDC hdc)
{
	COLORREF color = _mouseOnFlag & (1 << SelectEasy) ? MOUSE_ON_COLOR : RGB(0, 0, 0);
	color = _levelSelected && s_gameLevel == LEVEL_EASY ? SELECTED_COLOR : color;
	Utils::WriteText(hdc, SELECT_LEVEL_EASY_POS, L"EASY", LEVEL_FONT_SIZE, color);
}

void EntranceScene::DrawSelectNormal(HDC hdc)
{
	COLORREF color =  _mouseOnFlag & (1 << SelectNormal) ? MOUSE_ON_COLOR : RGB(0, 0, 0);
	color = _levelSelected && s_gameLevel == LEVEL_NORMAL ? SELECTED_COLOR : color;
	Utils::WriteText(hdc, SELECT_LEVEL_NORMAL_POS, L"NORMAL", LEVEL_FONT_SIZE, color);
	
}

void EntranceScene::DrawSelectHard(HDC hdc)
{
	COLORREF color = _mouseOnFlag & (1 << SelectHard) ? MOUSE_ON_COLOR : RGB(0, 0, 0);
	color = _levelSelected && s_gameLevel == LEVEL_HARD ? SELECTED_COLOR : color;
	Utils::WriteText(hdc, SELECT_LEVEL_HARD_POS, L"HARD", LEVEL_FONT_SIZE, color);
	
}
