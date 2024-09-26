#include "pch.h"
#include "Bishop.h"
#include "SceneManager.h"
#include "Board.h"

Bishop::Bishop(Board* board, ColorType color)
	: Super(board, TYPE_BISHOP, color)
{
}

Bishop::~Bishop()
{
}

void Bishop::GetAvailableMoves(OUT vector<Move>& moves)
{
	Super::GetAvailableMoves(moves, DIAGONAL_OFFSET, 4);
}

void Bishop::GetAttackZone(OUT Zone* zone)
{
	Super::CalculateAttackZone(DIAGONAL_OFFSET, 4, zone);
}

bool Bishop::CanMove(VectorInt fromIdx, VectorInt toIdx) const
{
	if (!Super::CanMove(fromIdx, toIdx))
		return false;

	VectorInt diff = toIdx - fromIdx;

	if (diff.x == 0 || diff.y == 0)
		return false;

	VectorInt offset(0, 0);
	offset.y = diff.y > 0 ? 1 : -1;
	offset.x = diff.x > 0 ? 1 : -1;

	VectorInt pos = fromIdx + offset;

	while (pos != toIdx)
	{
		if (!_board->IsValidIdx(pos))
			return false;
		if (_board->GetPieceOnIdxOrNull(pos) != nullptr)
			return false;

		pos += offset;
	}

	ASSERT_CRASH(pos == toIdx);

	return true;
}

