#include "pch.h"
#include "Knight.h"
#include "SceneManager.h"
#include "Board.h"


Knight::Knight(Board* board, ColorType color)
	: Super(board, TYPE_KNIGHT, color)
{
}

Knight::~Knight()
{
}

void Knight::GetAvailableMoves(OUT vector<Move>& moves)
{
	const VectorInt from = GetPosIdx();
	VectorInt to;

	for (int32 offsetIdx = 0; offsetIdx < 8; ++offsetIdx)
	{
		to = from + KNIGHT_OFFSET[offsetIdx];

		if (CanMove(from, to) && IsLegalMove(this, from, to))
		{
			Piece* captured = _board->GetPieceOnIdxOrNull(to);
			moves.push_back(Move(from, to, captured));
		}
	}
}

void Knight::GetAttackZone(OUT Zone* zone)
{
	const VectorInt& from = GetPosIdx();
	VectorInt to;

	for (int32 i = 0; i < 8; ++i)
	{
		to = from + KNIGHT_OFFSET[i];
		int32 idx = to.y * G_TILE_COUNT + to.x;

		if (Super::CanMove(from, to))
		{
			zone[idx] = Z_Red;

			Piece* p = _board->GetPieceOnIdxOrNull(to);

			if (p && p->GetType() == TYPE_KING)
				zone[from.y * G_TILE_COUNT + from.x] = Z_Orange;

		}
	}
}

bool Knight::CanMove(VectorInt fromIdx, VectorInt toIdx) const
{
	if (!Super::CanMove(fromIdx, toIdx))
		return false;

	for (int32 i = 0; i < 8; ++i)
	{
		VectorInt targetPos = fromIdx + KNIGHT_OFFSET[i];
		if (targetPos == toIdx)
			return true;
	}

	return false;
}
