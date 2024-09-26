#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd;
}

void InputManager::Update()
{
	BYTE getStates[MAX_KEY_STATE];
	
	if (!::GetKeyboardState(getStates))
		ASSERT_CRASH(false);

	for (int32 i = 0; i < MAX_KEY_STATE; ++i)
	{
		if (getStates[i] & 0x80)
		{
			if (_states[i] == KeyState::Down)
			{
				_states[i] = KeyState::Press;
				
			}
			else if (_states[i] == KeyState::None || _states[i] == KeyState::Up)
			{
				_states[i] = KeyState::Down;
			}
		}
		else
		{
			if (_states[i] == KeyState::Press || _states[i] == KeyState::Down)
			{
				_states[i] = KeyState::Up;
			}
			else if (_states[i] == KeyState::Up)
			{
				_states[i] = KeyState::None;
			}
		}
	}

	::GetCursorPos(&_mousePos);
	::ScreenToClient(_hwnd, &_mousePos);

}

void InputManager::Render(HDC hdc)
{
	const wstring mouseInfo = std::format(L"Mouse : X=({0:3}), Y=({1:3})", (int)_mousePos.x, (int)_mousePos.y);
	Utils::WriteText(hdc, { MOUSE_POS_BOX_X, MOUSE_POS_BOX_Y }, mouseInfo);
}

bool InputManager::IsCursorInCircle(const VectorInt& pos, const float radius) const
{
	VectorInt v = { _mousePos.x - pos.x, _mousePos.y - pos.y };
	float lengthSquared = v.LengthSquared();

	return lengthSquared < radius * radius;
}

bool InputManager::IsCursorInBox(const VectorInt& leftTop, const VectorInt& rightBottom) const
{
	return _mousePos.x > leftTop.x && _mousePos.y > leftTop.y &&
		_mousePos.x < rightBottom.x && _mousePos.y < rightBottom.y;
}
