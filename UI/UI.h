#pragma once
#include "Actor.h"

class Texture;

enum UI_Type
{
	UI_Square,
	UI_Circle,
	UI_Texture,

};

class UI : public Actor
{
	using Super = Actor;
	using FuncType = std::function<void()>;

public:
	UI(UI_Type uiType) : Super(LAYER_UI), _uiType(uiType) {}
	virtual ~UI() {};

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

	bool IsMouseOnBox() const { return _isMouseOnBox; }
	void SetFunc(FuncType func) { _func = func; }
	void Execute() { if (_func) _func(); }

protected:
	UI_Type _uiType;
	bool _isMouseOnBox = false;
	FuncType _func = nullptr;
};

