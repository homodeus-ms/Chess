#pragma once
#include "Piece.h"

class Pawn : public Piece
{
	using Super = Piece;
public:
	Pawn(Board* board, ColorType color);
	virtual ~Pawn() {}

	
	virtual bool CanMove(VectorInt fromIdx, VectorInt toIdx) const override;
	virtual void GetAvailableMoves(OUT vector<Move>& moves) override;
	virtual void GetAttackZone(OUT Zone* zone) override;

	inline bool IsCenterZone(const VectorInt& to) { return to.x > 1 && to.x < 6 && to.y >2 && to.y < 5; }
	inline bool IsNeverMoved() const;
	int32 CalculatePawnMoveScore(const VectorInt& from, const VectorInt& to);

private:
	bool IsMovingForward(const VectorInt& from, const VectorInt& to) const;
	
private:
	enum { MOVE_FORWARD_SCORE = 20, GET_CENTER_ZONE_SCORE = 15, };
	enum { PLAYER_START_POS_Y = 6, AI_START_POS_Y = 1 };
	
	enum { PROMOTION_SPOT_SCORE = 400, PRE_PROMOTION_SPOT_SCORE = 50 };
	enum 
	{ 
		PLAYER_OFFSET_START = 0, 
		PLAYER_OFFSET_END = 3, 
		AI_OFFSET_START = 4, 
		AI_OFFSET_END = 7, 
	};
};

