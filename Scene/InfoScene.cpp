#include "pch.h"
#include "InfoScene.h"
#include "InputManager.h"
#include "SceneManager.h"

void InfoScene::Init()
{
}

void InfoScene::Update()
{
	if (GET_SINGLE(InputManager)->IsKeyDown(SPACE))
	{
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::DevScene);
		Sleep(100);
		return;
	}
}

void InfoScene::Render(HDC hdc)
{
	Utils::WriteText(hdc, { 50, 50 }, L"INFO", 100);
	Utils::WriteText(hdc, { 500, 620 }, L"��� �Ϸ��� �����̽� �ٸ� ��������", 30);
}
