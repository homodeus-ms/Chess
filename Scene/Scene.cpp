#include "pch.h"
#include "Scene.h"
#include "Actor.h"

Scene::Scene(SceneType sceneType)
	: _sceneType(sceneType)
{
}

Scene::~Scene()
{
	for (vector<Actor*> actors : _actors)
	{
		for (Actor* a : actors)
		{
			SAFE_DELETE(a);
		}
	}
}

void Scene::Init()
{
	for (vector<Actor*> actors : _actors)
	{
		for (Actor* a : actors)
		{
			a->Init();
		}
	}
}

void Scene::Update()
{
	for (vector<Actor*> actors : _actors)
	{
		for (Actor* a : actors)
		{
			a->Update();
		}
	}
}

void Scene::Render(HDC hdc)
{
	for (vector<Actor*> actors : _actors)
	{
		for (Actor* a : actors)
		{
			a->Render(hdc);
		}
	}
}

SceneType Scene::GetCurrSceneType() const
{
	ASSERT_CRASH(_sceneType != SceneType::None);

	return _sceneType;
}

void Scene::AddActor(Actor* actor)
{
	if (actor == nullptr)
		return;
	_actors[actor->GetLayerType()].push_back(actor);
}

void Scene::RemoveToTrash(Actor* actor)
{
	if (actor == nullptr)
		return;
	
}
