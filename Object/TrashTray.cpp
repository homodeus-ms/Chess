#include "pch.h"
#include "TrashTray.h"
#include "Piece.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Texture.h"

TrashTray::TrashTray(Texture* texture, bool isPlayers)
	: Super(LAYER_BACKGROUND)
	, _isPlayers(isPlayers)
{
	SetTexture(texture);
	_trashes.reserve(16);

	VectorInt diff(140, 140);
	
	if (_isPlayers)
	{
		_leftTop = G_PLAYER_TRASH_TRAY_POS - diff;
		_rightBottom = G_PLAYER_TRASH_TRAY_POS + diff - VectorInt(24, 24);
	}
	else
	{
		_leftTop = G_AI_TRASH_TRAY_POS - diff;
		_rightBottom = G_AI_TRASH_TRAY_POS + diff - VectorInt(24, 24);
	}

	_pickingTexture = GET_SINGLE(ResourceManager)->GetTexture("RedBox");
	ASSERT_CRASH(_pickingTexture != nullptr);
}

void TrashTray::Init()
{
}

void TrashTray::Update()
{
}

void TrashTray::Render(HDC hdc)
{
	if (_isPicking)
	{
		SetTexture(GET_SINGLE(ResourceManager)->GetTexture("PromotionTray"));
	}
	Super::Render(hdc);

	

	for (Piece* p : _trashes)
	{
		p->Render(hdc);
	}

	if (_isPicking && IsValidPos(_drawPosLeftTop))
	{
		int32 sizeX = G_PIECE_SIZE;
		int32 sizeY = G_PIECE_SIZE;

		BLENDFUNCTION blendFunction;
		blendFunction.BlendOp = AC_SRC_OVER;       // ���� ä���� ����� �̹����� �׸��ϴ�.
		blendFunction.BlendFlags = 0;              // ������ �����Ƿ� 0���� �����մϴ�.
		blendFunction.SourceConstantAlpha = 40;   // �̹����� ����. (0: ���� ����, 255: ���� ������)
		blendFunction.AlphaFormat = AC_SRC_ALPHA;  // �ҽ� �̹����� ���� ä���� �����ϰ� �ֽ��ϴ�.

		int32 startX = _drawPosLeftTop.x;
		int32 startY = _drawPosLeftTop.y;

		AlphaBlend(hdc, startX, startY,
			sizeX, sizeY, _pickingTexture->GetDC(), 0, 0, sizeX, sizeY, blendFunction);
	}
}

void TrashTray::DeleteObjects()
{
	for (Piece* p : _trashes)
		SAFE_DELETE(p);
	_trashes.clear();
}

void TrashTray::AddTrash(Piece* piece, bool isPromotedPawn)
{
	if (piece == nullptr)
		return;

	if (isPromotedPawn)
	{
		uint32 idx = 0;
		for (Piece* p : _trashes)
		{
			p->SetPos(GetTrayPos(idx++));
		}
	}

	uint32 idx = static_cast<int32>(_trashes.size());
	const VectorInt& pos = GetTrayPos(idx);
	
	piece->SetPosIdx({ -1, -1 });
	piece->SetPos(pos);

	_trashes.push_back(piece);
}

void TrashTray::RemovePieceInTray(Piece* piece)
{
	auto findIt = std::find(_trashes.begin(), _trashes.end(), piece);
	
	ASSERT_CRASH(findIt != _trashes.end());
	_trashes.erase(findIt);
}

Piece* TrashTray::PickPieceForPromotionOrNull()
{
	_isPicking = true;

	VectorInt mousePos = GET_SINGLE(InputManager)->GetMousePos();

	if (IsValidPos(mousePos))
	{
		mousePos -= _leftTop;
		const int32 col = (mousePos.x - 1) / 64;
		const int32 row = (mousePos.y - 1) / 74;    // TILE_SIZE(64) + YDIFF(10)
		const int32 yDiff = row * 10;

		// ���� ���� �ڽ��� �׷����� LeftTop ��ġ�� ��
		_drawPosLeftTop = { _leftTop.x + col * G_PIECE_SIZE, _leftTop.y + row * G_PIECE_SIZE + yDiff};

		if (GET_SINGLE(InputManager)->IsKeyDown(LClick))
		{
			Piece* p = GetClickedPieceOrNull();
			if (p != nullptr)
			{
				_isPicking = false;
				RemovePieceInTray(p);

				SetTexture(GET_SINGLE(ResourceManager)->GetTexture("PlayerTray"));

				return p;
			}
		}
		return nullptr;
	}
	else
	{
		_drawPosLeftTop = { -1, -1 };
		return nullptr;
	}
	
}

Piece* TrashTray::GetPromotionPieceForAi()
{
	sort(_trashes.begin(), _trashes.end(), [](Piece* a, Piece* b) {
		int32 valueA = PIECE_VALUE[a->GetType()];
		int32 valueB = PIECE_VALUE[b->GetType()];
		return valueA < valueB;
		});

	Piece* ret = _trashes.back();
	_trashes.erase(_trashes.end() - 1, _trashes.end());

	return ret;
}

const VectorInt TrashTray::GetTrayPos(uint32 index)
{
	int32 col = index % 4;
	int32 row = index / 4;
	int32 yOffset = row * 10;

	const VectorInt pos = {
		_leftTop.x + col * G_PIECE_SIZE + G_PIECE_SIZE / 2,
		_leftTop.y + row * G_PIECE_SIZE + G_PIECE_SIZE / 2 + yOffset };

	return pos;
}

bool TrashTray::IsValidPos(const VectorInt& pos) const
{
	return pos >= _leftTop && pos <= _rightBottom;
}

Piece* TrashTray::GetClickedPieceOrNull()
{
	ASSERT_CRASH(_drawPosLeftTop != VectorInt(-1, -1));

	for (Piece* p : _trashes)
	{
		VectorInt pos = p->GetPos();
		pos.x -= G_PIECE_SIZE / 2;
		pos.y -= G_PIECE_SIZE / 2;
		if (pos == _drawPosLeftTop)
			return p;
	}

	return nullptr;
}
