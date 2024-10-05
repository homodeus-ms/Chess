#pragma once
#include "Player.h"
#include "Piece.h"
#include <random>

class Board;

enum MinMaxPlayer
{
	MIN_PLAYER,
	MAX_PLAYER,
};


class AI : public Player
{
	using Super = Player;
public:
	AI(ColorType color, Board* board, const GameLevel gameLevel);
	virtual void Update() override;

private:
	void CopyBoard();
	int32 MiniMax(int32 depth, int32 startDepth, int32 alpha, int32 beta, bool maxPlayer);
	void GetAvailableMoves(bool maxPlayer, vector<Move>& moves);

	int32 CalculateScore(const Move& move);
	int32 CalculatePositionBonus(const Move& move);
	void ApplyScore(bool isAiTurn, int32 score);
	int32 Evaluate();

private:
	int32 GetDepthLevelForThisTurn();
	int32 GetMyScore() { return _myScore; }
	int32 GetPlayerScore() { return _playerScore; }

private:

	enum
	{
		INIT_SCORE = 5000,
		RETURN_SCORE_LIMIT = -4000,
		TIME_LIMIT_TICK = 1000,
	};

	enum
	{
		POSITION_BASIC_BONUS = 10,
	};
	
	Piece* _currBoard[G_TILE_COUNT * G_TILE_COUNT] = { nullptr, };
	const vector<Piece*>& _myPieces;
	const vector<Piece*>& _playerPieces;
	
	uint64 _prevTime = 0;
	vector<Move> _bestMoves;
	bool _timeOut = false;
	
	const GameLevel _gameLevel;

	int32 _myScore = INIT_SCORE;
	int32 _playerScore = INIT_SCORE;

	std::mt19937 _randomNumGenerator;
	std::uniform_int_distribution<int> _rand;
};

