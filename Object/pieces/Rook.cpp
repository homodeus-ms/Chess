#include "pch.h"
#include "Rook.h"


Rook::Rook(Board* board, ColorType color)
	: Super(board, TYPE_ROOK, color)
{
}

Rook::~Rook()
{
}

void Rook::GetAvailableMoves(OUT vector<Move>& moves)
{
	Super::GetAvailableMoves(moves, STRAIGHT_OFFSET, 4);
}

void Rook::GetAttackZone(OUT Zone* zone)
{
	Super::CalculateAttackZone(STRAIGHT_OFFSET, 4, zone);
}

bool Rook::CanMove(VectorInt fromIdx, VectorInt toIdx) const
{
	if (!Super::CanMove(fromIdx, toIdx))
		return false;

	VectorInt diff = toIdx - fromIdx;

	if (diff.x != 0 && diff.y != 0)
		return false;

	VectorInt offset(0, 0);
	if (diff.x == 0)
		offset.y = diff.y > 0 ? 1 : -1;
	if (diff.y == 0)
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
