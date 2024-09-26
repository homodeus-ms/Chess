#pragma once
#include "Player.h"
#include "Piece.h"

class Human : public Player
{
	using Super = Player;
public:
	Human(ColorType color, Board* board) : Super(PlayerType::Human, color, board) {}
	virtual void Update() override;
	

private:
	Piece* _grabPiece = nullptr;
	VectorInt _grabStartIdx;
	vector<Move> _availableMoves;
	
};

