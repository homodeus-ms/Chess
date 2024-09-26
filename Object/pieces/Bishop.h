#pragma once
#include "Piece.h"

class Mover;

class Bishop : public Piece
{
	using Super = Piece;
public:
	Bishop(Board* board, ColorType color);
	virtual ~Bishop();

	
	virtual void GetAvailableMoves(OUT vector<Move>& moves) override;
	virtual void GetAttackZone(OUT Zone* zone) override;
	virtual bool CanMove(VectorInt fromIdx, VectorInt toIdx) const override;

private:
};

