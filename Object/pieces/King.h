#pragma once
#include "Piece.h"

class Mover;

class King : public Piece
{
	using Super = Piece;
public:
	King(Board* board, ColorType color);
	virtual ~King();

	virtual void GetAvailableMoves(OUT vector<Move>& moves) override;
	virtual void GetAttackZone(OUT Zone* zone) override;
	virtual bool CanMove(VectorInt fromIdx, VectorInt toIdx) const override;
private:
};

