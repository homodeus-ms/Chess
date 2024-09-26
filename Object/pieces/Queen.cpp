#include "pch.h"
#include "Queen.h"

Queen::Queen(Board* board, ColorType color)
	: Super(board, TYPE_QUEEN, color)
	
{
}

Queen::~Queen()
{
}

void Queen::GetAvailableMoves(OUT vector<Move>& moves)
{
	Super::GetAvailableMoves(moves, COMPOSITE_OFFSET, 8);
}

void Queen::GetAttackZone(OUT Zone* zone)
{
	Super::CalculateAttackZone(COMPOSITE_OFFSET, 8, zone);
}

bool Queen::CanMove(VectorInt fromIdx, VectorInt toIdx) const
{
	if (!Super::CanMove(fromIdx, toIdx))
		return false;

	VectorInt diff = toIdx - fromIdx;
	VectorInt offset(0, 0);
	if (diff.x != 0)
		offset.x = diff.x > 0 ? 1 : -1;
	if (diff.y != 0)
		offset.y = diff.y > 0 ? 1 : -1;

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

