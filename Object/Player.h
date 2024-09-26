#pragma once
#include "Piece.h"
class Board;

class Player
{
public:
	Player(PlayerType type, ColorType color, Board* board);
	~Player();

	virtual void Update();

	ColorType GetPlayerColor() const { return _color; }
	PlayerType GetType() const { return _playerType; }
	const Move& GetLastMove() const { return _bestMove; }
	void ResetMove() { _bestMove.Reset(); }

protected:
	PlayerType _playerType;
	ColorType _color;
	uint32 _currScore = 1400;

	Board* _board = nullptr;
	Move _bestMove;
};

