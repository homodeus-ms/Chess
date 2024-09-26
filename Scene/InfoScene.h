#pragma once
#include "GameScene.h"
class InfoScene : public GameScene
{
	using Super = GameScene;
public:

	InfoScene() : Super(SceneType::InfoScene) {};
	virtual ~InfoScene() {}

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	
	
};

