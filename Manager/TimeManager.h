#pragma once

class TimeManager
{
	DECLARE_SINGLE(TimeManager);

public:
	void Init();
	void Update();
	void Render(HDC hdc);

	float GetDeltaTime() const { return _deltaTime; }

private:
	
	enum
	{
		TIME_INFO_POS_X = G_WINSIZE_X - 260,
		TIME_INFO_POS_Y = 10,
	};

	float _deltaTime = 0.f;
	uint64 _prevCount = 0;
	uint32 _frameCount = 0;
	uint64 _frequency = 0;
	uint32 _fps = 0;
	float _frameTime = 0.f;
};

