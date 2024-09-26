#pragma once
#include "Scene.h"


class GameScene : public Scene
{
	using Super = Scene;
public:

	GameScene(SceneType sceneType) : Super(sceneType) {};
	virtual ~GameScene() {}

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

	static const ColorType GetPlayerColor() { return s_playerColor; }
	static const GameLevel GetGameLevel() { return s_gameLevel; }

protected:
	static ColorType s_playerColor;
	static GameLevel s_gameLevel;
};

