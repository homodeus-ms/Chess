#pragma once
#include "Piece.h"
#include "SceneManager.h"
#include "Board.h"

class Mover;

class Queen : public Piece
{
	using Super = Piece;
public:
	Queen(Board* board, ColorType color);
	virtual ~Queen();
	
	virtual void GetAvailableMoves(OUT vector<Move>& moves) override;
	virtual void GetAttackZone(OUT Zone* zone) override;
	virtual bool CanMove(VectorInt fromIdx, VectorInt toIdx) const override;
private:
};

