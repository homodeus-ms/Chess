#include "pch.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "EntranceScene.h"
#include "InfoScene.h"
#include "GameScene.h"

SceneManager::~SceneManager()
{
	for (Scene* s : _scenes)
	{
		SAFE_DELETE(s);
	}
}

void SceneManager::Init()
{
	_scenes[0] = nullptr;
	_scenes[1] = new EntranceScene();
	_scenes[2] = new InfoScene();
	_scenes[3] = new DevScene();
	//Scene* devScene = new DevScene(ColorType::Black, 3);
	
	_currScene = _scenes[1];

	_currScene->Init();
}

void SceneManager::Update()
{
	_currScene->Update();
}

void SceneManager::Render(HDC hdc)
{
	_currScene->Render(hdc);
}

Scene* SceneManager::ChangeScene(SceneType sceneType)
{
	if (sceneType == _currScene->GetCurrSceneType())
		return _currScene;

	if (sceneType == SceneType::DevScene)
	{
		ASSERT_CRASH(_currScene == _scenes[2]);
		_scenes[3]->Init();
	}
	_currScene = _scenes[static_cast<int32>(sceneType)];

	return _currScene;
}

DevScene* SceneManager::GetDevScene() const
{
	ASSERT_CRASH(_scenes[3] != nullptr);

	return static_cast<DevScene*>(_scenes[3]);
}
