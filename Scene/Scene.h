#pragma once

enum class SceneType
{
	None,
	EntranceScene,
	InfoScene,
	DevScene,
};

class Actor;
class UI;

class Scene
{
public:
	Scene(SceneType sceneType);
	virtual ~Scene();

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

	SceneType GetCurrSceneType() const;

	void AddActor(Actor* actor);
	void RemoveToTrash(Actor* actor);
	

protected:
	SceneType _sceneType;
	vector<Actor*> _actors[LAYER_MAX_COUNT];
	vector<Actor*> _trashes;
	
};

