#include "pch.h"
#include "Piece.h"
#include "Board.h"
#include "Texture.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "TrashTray.h"



/*-----------*/
//  STATICS 
/*-----------*/

const VectorInt* Piece::STRAIGHT_OFFSET = new VectorInt[4] {
	VectorInt(-1, 0),
	VectorInt(1, 0),
	VectorInt(0, -1),
	VectorInt(0, 1),
};
const VectorInt* Piece::DIAGONAL_OFFSET = new VectorInt[4] {
	VectorInt(-1, -1),
	VectorInt(-1, 1),
	VectorInt(1, -1),
	VectorInt(1, 1),
};
const VectorInt* Piece::KNIGHT_OFFSET = new VectorInt[8] {
	{-2, -1}, {-1, -2}, {-2, 1}, {-1, 2},
		{2, -1}, {1, -2}, {2, 1}, {1, 2}
};
// [0 ~ 3] : BottomToTop  |  [4 ~ 7] : TopToBottom 
const VectorInt* Piece::PAWN_OFFSET = new VectorInt[8] {
	{0, -1}, {-1, -1}, {1, -1}, {0, -2}, {0, 1}, {-1, 1}, {1, 1}, {0, 2}
};
const VectorInt* Piece::COMPOSITE_OFFSET = new VectorInt[8] {
	VectorInt(-1, 0),
	VectorInt(1, 0),
	VectorInt(0, -1),
	VectorInt(0, 1),
	VectorInt(-1, -1),
	VectorInt(-1, 1),
	VectorInt(1, -1),
	VectorInt(1, 1),
};


Piece::Piece(Board* board, PieceType type, ColorType color)
	: Super(LAYER_OBJECT)
	, _pieceType(type)
	, _color(color)
	, _board(board)
	, _boardState(board->GetBoard())
	
{
	int32 x = G_BOARD_STARTX;
	int32 y = G_BOARD_STARTY;
	x += _posIdx.x * G_TILE_SIZE + G_TILE_SIZE / 2;
	y += _posIdx.y * G_TILE_SIZE + G_TILE_SIZE / 2;
	SetPos({ x, y });
	
}

Piece::~Piece()
{
	
	SAFE_DELETE(STRAIGHT_OFFSET);
	SAFE_DELETE(DIAGONAL_OFFSET);
	SAFE_DELETE(COMPOSITE_OFFSET);
	SAFE_DELETE(KNIGHT_OFFSET);
	SAFE_DELETE(PAWN_OFFSET);

	
}

void Piece::Init()
{
	Super::Init();
}

void Piece::Update()
{
	Super::Update();
}

void Piece::Render(HDC hdc)
{
	//Super::Render(hdc);
	Texture* texture = GetTexture();
	int32 sizeX = texture->GetSize().x;
	int32 sizeY = texture->GetSize().y;

	BLENDFUNCTION blendFunction;
	blendFunction.BlendOp = AC_SRC_OVER;       // 알파 채널을 사용해 이미지를 그립니다.
	blendFunction.BlendFlags = 0;              // 사용되지 않으므로 0으로 설정합니다.
	blendFunction.SourceConstantAlpha = 255;   // 이미지의 투명도. (0: 완전 투명, 255: 완전 불투명)
	blendFunction.AlphaFormat = AC_SRC_ALPHA;  // 소스 이미지는 알파 채널을 포함하고 있습니다.

	AlphaBlend(hdc, GetPos().x - sizeX / 2, GetPos().y - sizeY / 2, 
		sizeX, sizeY, texture->GetDC(), 0, 0, sizeX, sizeY, blendFunction);
}



bool Piece::CanMove(VectorInt fromIdx, VectorInt toIdx) const
{
	if (!_board->IsValidIdx(toIdx))
		return false;
	if (fromIdx == toIdx)
		return false;
	Piece* get = _board->GetPieceOnIdxOrNull(toIdx);
	if (get && _color == get->GetPieceColor())
		return false;

	return true;
}


void Piece::SetPosIdx(const VectorInt& pos)
{
	_posIdx = pos;
	int32 x = G_BOARD_STARTX;
	int32 y = G_BOARD_STARTY;
	x += _posIdx.x * G_TILE_SIZE + G_TILE_SIZE / 2;
	y += _posIdx.y * G_TILE_SIZE + G_TILE_SIZE / 2;
	SetPos({ x, y });
}

bool Piece::IsPlayers() const
{
	return _color == GET_DEVSCENE->GetPlayerColor();
}

void Piece::GetAvailableMoves(OUT vector<Move>& moves, const VectorInt* offset, int32 offsetLength)
{
	const VectorInt from = GetPosIdx();
	VectorInt to;
	int32 maxMoveCount = GetType() == TYPE_KING ? 1 : 7;

	for (int32 offsetIdx = 0; offsetIdx < offsetLength; ++offsetIdx)
	{
		to = from;

		for (int32 moveCount = 0; moveCount < maxMoveCount; ++moveCount)
		{
			to += offset[offsetIdx];
			bool canMove = CanMove(from, to);

			Piece* captured = _board->GetPieceOnIdxOrNull(to);
			
			if (canMove)
			{
				if (captured && captured->GetType() == TYPE_KING)
				{
					moves.push_back(Move(from, to, captured));
					return;
				}

				if (IsLegalMove(this, from, to))
				{
					moves.push_back(Move(from, to, captured));
				}
			}
			else
			{
				break;
			}
		}
	}
}



/*------------------------------------------------------------------------*/
//                   CalculateAttackZone 함수
// 
// Red : 공격가능한 자리들
// Orange : B, Q, R 등에 해당, King으로 공격 오는 다이렉트 루트
// Yellow : B, Q, R 등이 King을 공격 할 수 있는 루트에 있는 피스
//               공격인자를 제거하지 않는 한 움직일 수 없음!
//
/*--------------------------------------------------------------------------*/

void Piece::CalculateAttackZone(const VectorInt* offset, int32 offsetLength, OUT Zone* zone)
{
	const VectorInt from = GetPosIdx();
	VectorInt to;
	ColorType targetColor = GetPieceColor() == ColorType::Black ? ColorType::White : ColorType::Black;
	
	vector<int32> indices;
	indices.reserve(8);
	
	for (int32 offsetIdx = 0; offsetIdx < offsetLength; ++offsetIdx)
	{
		to = from;
		indices.clear();

		for (int32 moveCount = 0; moveCount < 8; ++moveCount)
		{
			to += offset[offsetIdx];
			int32 idx = _board->VectorToIdx(to);

			if (Piece::CanMove(from, to))
			{
				indices.push_back(idx);
				Piece* p = _board->GetPieceOnIdxOrNull(to);

				if (p != nullptr)
				{
					if (p->GetType() == TYPE_KING)
					{
						FillColor(indices, Z_Orange, zone);
						zone[from.y * G_TILE_COUNT + from.x] = Z_Orange;
						break;
					}
					else if (IsKingBehind(to, offset[offsetIdx], targetColor))
					{
						FillColor(indices, Z_Red, zone);
						zone[idx] = Z_Yellow;
						break;
					}
					else
					{
						FillColor(indices, Z_Red, zone);
						break;
					}
				}
			}
			else
			{
				if (!indices.empty())
				{
					FillColor(indices, Z_Red, zone);
				}
				break;
			}
		}
	}
}

bool Piece::IsKingBehind(const VectorInt& from, const VectorInt& offset, ColorType targetColor)
{
	VectorInt to = from + offset;
	while (true) 
	{
		if (!_board->IsValidIdx(to))
			break;

		Piece* p = _board->GetPieceOnIdxOrNull(to);
		if (p != nullptr)
		{
			if (p->GetType() == TYPE_KING && p->GetPieceColor() == targetColor)
				return true;
			else
				return false;
		}

		to += offset;
	}

	return false;
}

void Piece::FillColor(const vector<int32>& indices, Zone zoneColor, OUT Zone* zone)
{
	for (int32 idx : indices)
		zone[idx] = zoneColor;
}


bool Piece::IsLegalMove(const Piece* movingPiece, const VectorInt& from, const VectorInt& to)
{
	Zone currZoneColor = _board->GetZoneColor(movingPiece->IsPlayers(), from);
	Zone targetZoneColor = _board->GetZoneColor(movingPiece->IsPlayers(), to);

	if (movingPiece->GetType() == TYPE_KING)
	{
		if (targetZoneColor == Z_Orange)
		{
			if (_board->GetPieceOnIdxOrNull(to))
				return true;
			else
				return false;
		}

		if (targetZoneColor == Z_Red)
			return false;
	}
	else if (_board->IsKingInCheckMate(movingPiece->GetPieceColor()) && currZoneColor != Z_Yellow)
	{
		if (targetZoneColor == Z_Orange)
			return true;
		return false;
	}
	else if (currZoneColor == Z_Yellow)
	{
		Move move;
		move.from = from;
		move.to = to;
		move.captured = _board->GetPieceOnIdxOrNull(to);
		bool ret = true;

		_board->MakeMove(move);
		_board->UpdateAttackZone(IsPlayers());

		if (_board->IsKingInCheckMate(GetPieceColor()))
			ret = false;

		_board->UnMove(move);
		_board->UpdateAttackZone(IsPlayers());

		return ret;
	}

	return true;
}



