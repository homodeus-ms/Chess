#include "pch.h"
#include "Pawn.h"
#include "SceneManager.h"
#include "Board.h"


Pawn::Pawn(Board* board, ColorType color)
	: Super(board, TYPE_PAWN, color)
{
}

bool Pawn::CanMove(VectorInt fromIdx, VectorInt toIdx) const
{
	if (!Super::CanMove(fromIdx, toIdx))
		return false;

	VectorInt diff = toIdx - fromIdx;
	diff *= IsPlayers() ? -1 : 1;
	const Piece* target = _board->GetPieceOnIdxOrNull(toIdx);
	
	if (abs(diff.x) == 1 && diff.y == 1)
	{
		return target == nullptr ? false : true;
	}
	if (!target && diff.x == 0)
	{
		if (diff.y == 1)
			return true;

		if (diff.y == 2 && IsNeverMoved())
		{
			int32 frontCellY = IsPlayers() ? fromIdx.y - 1 : fromIdx.y + 1;
			Piece* frontTarget = _board->GetPieceOnIdxOrNull({ fromIdx.x, frontCellY });
			return frontTarget == nullptr ? true : false;
		}
	}

	return false;

}

void Pawn::GetAvailableMoves(OUT vector<Move>& moves)
{
	const VectorInt from = GetPosIdx();
	VectorInt to;

	bool isPlayers = IsPlayers();
	int32 offsetStartIdx = isPlayers ? PLAYER_OFFSET_START : AI_OFFSET_START;
	int32 offsetLastIdx = isPlayers ? PLAYER_OFFSET_END : AI_OFFSET_END;

	for (int32 i = offsetStartIdx; i < offsetLastIdx; ++i)
	{
		to = from + PAWN_OFFSET[i];
		if (CanMove(from, to) && IsLegalMove(this, from, to))
		{
			Piece* captured = _board->GetPieceOnIdxOrNull(to);
			moves.push_back(Move(from, to, captured));
		}
	}

	// if never moved
	if (IsNeverMoved())
	{
		to = from + PAWN_OFFSET[offsetLastIdx];
		if (CanMove(from, to) && IsLegalMove(this, from, to))
		{
			moves.push_back(Move(from, to, nullptr));
		}
	}
}

void Pawn::GetAttackZone(OUT Zone* zone)
{
	const VectorInt from = GetPosIdx();
	VectorInt to;

	bool isPlayers = IsPlayers();
	
	int32 offsetStartIdx = isPlayers ? PLAYER_OFFSET_START : AI_OFFSET_START;
	int32 offsetLastIdx = isPlayers ? PLAYER_OFFSET_END : AI_OFFSET_END;

	for (int32 i = 1; i <= 2; ++i)
	{
		to = from + VectorInt(PAWN_OFFSET[offsetStartIdx + i]);
		int32 idx = to.y * G_TILE_COUNT + to.x;

		if (_board->IsValidIdx(to))
		{
			Piece* p = _board->GetPieceOnIdxOrNull(to);
			
			if (p == nullptr || p->GetPieceColor() != GetPieceColor())
			{
				zone[idx] = Z_Red;

				if (p && p->GetType() == TYPE_KING)
					zone[from.y * G_TILE_COUNT + from.x] = Z_Orange;
			}
		}
	}
}

inline bool Pawn::IsNeverMoved() const
{
	if (IsPlayers())
		return _posIdx.y == PLAYER_START_POS_Y;

	return _posIdx.y == AI_START_POS_Y;
}

int32 Pawn::CalculatePawnMoveScore(const VectorInt& from, const VectorInt& to)
{
	// Promotion Score
	if (IsPlayers() && to.y <= 1)
		return to.y == 0 ? PROMOTION_SPOT_SCORE : PRE_PROMOTION_SPOT_SCORE;
	else if (to.y >= 6)
		return to.y == 7 ? PROMOTION_SPOT_SCORE : PRE_PROMOTION_SPOT_SCORE;

	if (to >= BONUS_AREA_LEFT_TOP && to <= BONUS_AREA_RIGHT_BOTTOM)
		return GET_CENTER_ZONE_SCORE;

	return IsMovingForward(from, to) ? MOVE_FORWARD_SCORE : 0;
}

bool Pawn::IsMovingForward(const VectorInt& from, const VectorInt& to) const
{
	if (IsPlayers() && GetPosIdx().y < to.y)
		return true;
	else if (!IsPlayers() && GetPosIdx().y > to.y)
		return true;

	return false;
}




