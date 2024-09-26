#pragma once
#include "Piece.h"
#include "SceneManager.h"
#include "Board.h"

class Mover;

class Rook : public Piece
{
	using Super = Piece;
public:
	Rook(Board* board, ColorType color);
	virtual ~Rook();
	
	virtual void GetAvailableMoves(OUT vector<Move>& moves) override;
	virtual void GetAttackZone(OUT Zone* zone) override;
	virtual bool CanMove(VectorInt fromIdx, VectorInt toIdx) const override;
private:
};


