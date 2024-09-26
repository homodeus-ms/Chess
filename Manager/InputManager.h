#pragma once

enum class KeyState
{
	None,
	Press,
	Down,
	Up,
	Count,
};

enum
{
	MAX_KEY_STATE = 256,
};

enum KeyType
{
	LClick = VK_LBUTTON,
	RClick = VK_RBUTTON,

	H = 'H',
	Z = 'Z',
	SPACE = VK_SPACE,
};

class InputManager
{
	DECLARE_SINGLE(InputManager);

public:
	void Init(HWND hwnd);
	void Update();
	void Render(HDC hdc);

	VectorInt GetMousePos() const
	{
		return VectorInt(_mousePos.x, _mousePos.y);
	}

	bool IsCursorInCircle(const VectorInt& pos, const float radius) const;
	bool IsCursorInBox(const VectorInt& leftTop, const VectorInt& rightBottom) const;

	bool IsKeyPressing(KeyType key) { return  _states[key] == KeyState::Press; }
	bool IsKeyDown(KeyType key) { return _states[key] == KeyState::Down; }
	bool IsKeyUp(KeyType key) { return _states[key] == KeyState::Up; }
	
private:
	enum { MOUSE_POS_BOX_X = 20, MOUSE_POS_BOX_Y = 10 };

	HWND _hwnd = {};
	KeyState _states[MAX_KEY_STATE] = { KeyState::None, };
	POINT _mousePos = {};
};

