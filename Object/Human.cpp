#include "pch.h"
#include "Human.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Board.h"
#include "TrashTray.h"

//#include "Game.h"

void Human::Update()
{
	Super::Update();

	ASSERT_CRASH(_board != nullptr);

	InputManager* inputManager = GET_SINGLE(InputManager);

	if (inputManager->IsKeyDown(LClick))
	{
		VectorInt pos = inputManager->GetMousePos();
		_grabStartIdx = _board->ChangePosToIdx(pos);
		_grabPiece = _board->GetPieceOnIdxOrNull(_grabStartIdx);

		if (_grabPiece)
		{
			_grabPiece->SetPos(pos);
			_grabPiece->GetAvailableMoves(_availableMoves);
			_board->SetAvailableMoves(&_availableMoves);
		}
	}
	else if (inputManager->IsKeyPressing(LClick) && _grabPiece)
	{
		VectorInt pos = inputManager->GetMousePos();
		_grabPiece->SetPos(pos);
	}
	// Drop ºÎºÐ
	else if (_grabPiece)
	{
		VectorInt posIdx = _board->ChangePosToIdx(_grabPiece->GetPos());

		if (_grabStartIdx == posIdx || !_grabPiece->CanMove(_grabStartIdx, posIdx))
		{
			_grabPiece->SetPosIdx(_grabStartIdx);
		}
		else
		{
			Piece* captured = _board->GetPieceOnIdxOrNull(posIdx);
			_bestMove = { _grabStartIdx, posIdx, captured };
			//_board->UpdatePiecePos(move);
		}

		_board->SetAvailableMoves(nullptr);
		_grabPiece = nullptr;
	}
}

