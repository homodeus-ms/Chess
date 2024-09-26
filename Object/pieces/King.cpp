#include "pch.h"
#include "King.h"
#include "SceneManager.h"
#include "Board.h"


King::King(Board* board, ColorType color)
	: Super(board, TYPE_KING, color)
{
}

King::~King()
{
}

void King::GetAvailableMoves(OUT vector<Move>& moves)
{
	Super::GetAvailableMoves(moves, COMPOSITE_OFFSET, 8);
}

void King::GetAttackZone(OUT Zone* zone)
{
	const VectorInt& from = GetPosIdx();
	VectorInt to;

	for (int32 i = 0; i < 8; ++i)
	{
		to = from + COMPOSITE_OFFSET[i];
		int32 idx = to.y * G_TILE_COUNT + to.x;
		
		if (Super::CanMove(from, to))
		{
			zone[idx] = Z_Red;
		}
	}
}

bool King::CanMove(VectorInt fromIdx, VectorInt toIdx) const
{
	if (!Super::CanMove(fromIdx, toIdx))
		return false;

	VectorInt diff = toIdx - fromIdx;
	if (abs(diff.x) > 1 || abs(diff.y) > 1)
		return false;

	return true;
}

