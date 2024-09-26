#pragma once

#include "Scene.h"

class DevScene;

class SceneManager
{
	DECLARE_SINGLE(SceneManager);
public:
	~SceneManager();

	void Init();
	void Update();
	void Render(HDC hdc);

	Scene* GetCurrentScene() const { return _currScene; }
	Scene* ChangeScene(SceneType sceneType);
	DevScene* GetDevScene() const;
	

private:
	
	// 0 = NONE, 1 = Entrance, 2 = Info, 3 = Dev
	Scene* _scenes[4] = { };
	Scene* _currScene = nullptr;
};

