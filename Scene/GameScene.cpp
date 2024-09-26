#include "pch.h"
#include "GameScene.h"

ColorType GameScene::s_playerColor = ColorType::Black;
GameLevel GameScene::s_gameLevel = LEVEL_EASY;

void GameScene::Init()
{
	Super::Init();
}

void GameScene::Update()
{
	Super::Update();
}

void GameScene::Render(HDC hdc)
{
	Super::Render(hdc);
}
