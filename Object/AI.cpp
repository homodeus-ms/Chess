#include "pch.h"
#include "AI.h"
#include "Board.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "DevScene.h"
#include "Pawn.h"

#include <ctime>



AI::AI(ColorType color, Board* board, const GameLevel gameLevel)
	: Super(PlayerType::Ai, color, board)
	, _myPieces(_board->GetMyPieces(PlayerType::Ai))
	, _playerPieces(_board->GetMyPieces(PlayerType::Human))
	, _gameLevel(gameLevel)
	, _randomNumGenerator(static_cast<unsigned int>(std::time(0)))
	, _rand(1, 100)
{
}

void AI::Update()
{
	Super::Update();

	CopyBoard();
	_myScore = INIT_SCORE;
	_playerScore = INIT_SCORE;

	int32 targetDepth = GetDepthLevelForThisTurn();
	
	int32 retValue;

	_prevTime = ::GetTickCount64();

	retValue = MiniMax(targetDepth, 1, INT_MIN, INT_MAX, true);

	if (retValue < RETURN_SCORE_LIMIT)
		_bestMove.Reset();

	_timeOut = false;

	uint64 now = ::GetTickCount64();

	while (now - _prevTime < TIME_LIMIT_TICK)
	{
		now = ::GetTickCount64();
	}

	if (_bestMove.IsEmpty())
	{
		GET_DEVSCENE->GiveUp();
		return;
	}
}


void AI::CopyBoard()
{
	Piece** currboard = _board->GetBoard();
	::memcpy(_currBoard, currboard, sizeof(Piece*) * G_TILE_COUNT * G_TILE_COUNT);
}


int32 AI::MiniMax(int32 targetDepth, int32 startDepth, int32 alpha, int32 beta, bool isMaxPlayer)
{
	if (startDepth == targetDepth)
	{
		return Evaluate();
	}

	vector<Move> moves;
	moves.reserve(200);

	GetAvailableMoves(isMaxPlayer, moves);

	if (moves.size() == 0)
		return Evaluate();

	if (isMaxPlayer)
	{
		int32 max = INT_MIN;

		for (Move& m : moves)
		{
			_board->MakeMove(m);
			int32 tempScore = CalculateScore(m);
			ApplyScore(true, tempScore);
			int32 retValue = MiniMax(targetDepth, startDepth + 1, alpha, beta, false);
			_board->UnMove(m);
			ApplyScore(true, -tempScore);
			
			if (retValue > max)
			{
				max = retValue;

				if (startDepth == 1)
					_bestMove = m;
			}
			
			alpha = max(alpha, max);

			if (beta <= alpha)
				break;
		}

		return max;
	}
	else
	{
		int32 min = INT_MAX;

		for (Move& m : moves)
		{
			
			_board->MakeMove(m);
			int32 tempScore = CalculateScore(m);
			ApplyScore(false, tempScore);
			int32 retValue = MiniMax(targetDepth, startDepth + 1, alpha, beta, true);
			_board->UnMove(m);
			ApplyScore(false, -tempScore);

			if (retValue < min)
				min = retValue;
			
			beta = min(beta, min);

			if (beta <= alpha)
				break;
		}

		return min;
	}
}

void AI::GetAvailableMoves(bool maxPlayer, vector<Move>& moves)
{
	if (maxPlayer)
	{
		// Ai피스들의 움직임을 계산해야하니까 Player피스들의 AttackZone 계산을 해야됨
		_board->UpdateAttackZone(true);

		for (Piece* p : _myPieces)
		{
			p->GetAvailableMoves(moves);
		}
	}
	else
	{
		_board->UpdateAttackZone(false);

		for (Piece* p : _playerPieces)
		{
			p->GetAvailableMoves(moves);
		}
	}

	std::sort(moves.begin(), moves.end(), [=](const Move& a, const Move& b) {
		int32 aScore = 0;
		int32 bScore = 0;
		if (a.captured != nullptr)
			aScore = CalculateScore(a);
		if (b.captured != nullptr)
			bScore = CalculateScore(b);
		return aScore > bScore;
		});
}


int32 AI::CalculateScore(const Move& move)
{
	int32 score = 0;
	 
	if (move.captured)
		score = PIECE_VALUE[move.captured->GetType()];

	score += CalculatePositionBonus(move);

	return score;
}

int32 AI::CalculatePositionBonus(const Move& move)
{
	int32 bonus = 0;
	Piece* p = _board->GetPieceOnIdxOrNull(move.to);

	if (p->GetType() == TYPE_PAWN)
	{
		Pawn* pawn = static_cast<Pawn*>(p);
		bonus += pawn->CalculatePawnMoveScore(move.from, move.to);
	}
	else if (move.to >= BONUS_AREA_LEFT_TOP && move.to <= BONUS_AREA_RIGHT_BOTTOM)
		bonus += POSITION_BASIC_BONUS;

	return bonus;
}

void AI::ApplyScore(bool isAiTurn, int32 score)
{
	if (isAiTurn)
		_myScore += score;
	else
		_playerScore += score;
}

int32 AI::Evaluate()
{
	int32 score = _myScore - _playerScore;


	return score;
}


int32 AI::GetDepthLevelForThisTurn()
{
	int32 ret = 3;
	int32 randomNum = _rand(_randomNumGenerator);

	switch (_gameLevel)
	{
	case LEVEL_EASY:
	{
		ret = randomNum % 5 == 0 ? 2 : 3;
		break;
	}
	case LEVEL_NORMAL:
	{
		if (randomNum % 20 == 0)
			ret = 5;
		else
			ret = 4;
		break;
	}
	case LEVEL_HARD:
	{
		ret = randomNum % 2 == 0 ? 6 : 7;
		break;
	}
	default:
		ASSERT_CRASH(false);
		break;
	}

	return ret;
}




