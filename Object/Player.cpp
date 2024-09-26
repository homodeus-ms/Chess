#include "pch.h"
#include "Player.h"
#include "Board.h"

Player::Player(PlayerType type, ColorType color, Board* board)
	: _playerType(type), _color(color), _board(board)
{
	_bestMove.Reset();
}

Player::~Player()
{
}

void Player::Update()
{
	
}

//Move Player::GetLastMove()
//{
//	if (_bestMove.IsEmpty())
//		return _bestMove;
//	
//	Move move = std::move(_bestMove);
//	_bestMove.Reset();
//
//	return move;
//}
