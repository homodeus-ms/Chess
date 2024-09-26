#include "pch.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "DevScene.h"
#include "Texture.h"
#include "Actor.h"
#include "Board.h"
#include "Piece.h"
#include "Player.h"
#include "Human.h"
#include "AI.h"
#include "TrashTray.h"
#include "UI.h"
#include "BoxUI.h"
#include "TexturedUI.h"


DevScene::DevScene()
	: Super(SceneType::DevScene)
	
{
	
}

DevScene::~DevScene()
{
}

void DevScene::Init()
{

	LoadImages();
	
	// Board
	{
		Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture("board", "chessboard2.png");
		Actor* board = new Board(texture, s_playerColor);
		Board* boardPtr = reinterpret_cast<Board*>(board);

		AddActor(board);
		_board = boardPtr;

		if (s_playerColor == ColorType::Black)
			_board->SetBoard(PLAYER_BLACK_BASIC_FORM, s_playerColor);
			//_board->SetBoard(TEST5, s_playerColor);
		else
			_board->SetBoard(PLAYER_WHITE_BASIC_FORM, s_playerColor);
			//_board->SetBoard(TEST2_WHITE, s_playerColor);


		//_board->SetBoard(PLAYER_BLACK_TEST2, _playerColor);
		//_board->SetBoard(BASIC_TEST1, _playerColor);
		//_board->SetBoard(BASIC_TEST2, _playerColor);
		//_board->SetBoard(GIVEUP_TEST2, _playerColor);
		//_board->SetBoard(TEST1, _playerColor);
	}


	// Player
	{
		_player = new Human(s_playerColor, _board);
		_ai = new AI(s_playerColor == ColorType::Black ? ColorType::White : ColorType::Black, _board, s_gameLevel);
		_currTurn = s_playerColor == ColorType::Black ? PLAYER_TURN : AI_TURN;
		_board->SetPlayer(_player);
		_board->SetPlayer(_ai);
	}

	// TrashTray
	{
		
		// Tray For AI
		{
			Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture("AiTray", "trashTray1.png");
			Actor* tray = new TrashTray(texture, false);
			tray->SetPos(G_AI_TRASH_TRAY_POS);
			AddActor(tray);
			_trashes[1] = static_cast<TrashTray*>(tray);
			_board->SetTrashTray(static_cast<TrashTray*>(tray), PlayerType::Ai);
		}
		//Tray For Player
		{
			Texture* texture = GET_SINGLE(ResourceManager)->LoadTexture("PlayerTray", "trashTray2.png");
			GET_SINGLE(ResourceManager)->LoadTexture("PromotionTray", "promotionTray.png");
			
			Actor* tray = new TrashTray(texture, true);
			tray->SetPos(G_PLAYER_TRASH_TRAY_POS);
			AddActor(tray);
			_trashes[0] = static_cast<TrashTray*>(tray);
			_board->SetTrashTray(static_cast<TrashTray*>(tray), PlayerType::Human);
		}
	}

	

	LoadUIs();

	Super::Init();
}

void DevScene::Update()
{
	if (_aiGiveup || IsGameOver())
	//if (false)
	{

		if (!_endingSoundPlayed)
		{
			if (_aiGiveup)
			{
				GET_SINGLE(ResourceManager)->PlaySounds("Win");
				_endingSoundPlayed = true;
			}
			else if (IsGameOver())
			{
				if (_currTurn == PLAYER_TURN)
					GET_SINGLE(ResourceManager)->PlaySounds("Lose");
				else
					GET_SINGLE(ResourceManager)->PlaySounds("Win");
				_endingSoundPlayed = true;
			}
		}
		return;
	}

	Super::Update();
}

void DevScene::Render(HDC hdc)
{
	Super::Render(hdc);
	
	const wstring COLOR_INFO = std::format(
		L"[ {0} ¡Ü  |  {1} ¡Û ]",
		s_playerColor == ColorType::Black ? L"Player" : L"AI",
		s_playerColor == ColorType::Black ? L"AI" : L"Player"
	);
	Utils::WriteText(hdc, { 324, 10 }, COLOR_INFO);

	const wstring TURN_INFO = std::format(L"TURN_NO.  {0}    Now -> {1}", _currTurnNo,
		_currTurn == PLAYER_TURN ? L"¡Ü" : L"¡Û");

	Utils::WriteText(hdc, { 520, 10 }, TURN_INFO);



	if (IsGameOver() || _aiGiveup)
	{
		const wstring giveup = L"AI Give Up!";
		const wstring str = _currTurn == AI_TURN ? L"YOU WIN!" : L"YOU LOSE!";
		if (_aiGiveup)
		{
			Utils::WriteText(hdc, { 300, 200 }, giveup, 150, RGB(220, 100, 100));
		}
		else
		{
			Utils::WriteText(hdc, { 300, 200 }, str, 150, RGB(220, 100, 100));
		}
	}
}

void DevScene::LoadImages()
{
	{
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("B_PAWN", "b_pawn.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("B_KING", "b_king.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("B_QUEEN", "b_queen.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("B_ROOK", "b_rook.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("B_BISHOP", "b_bishop.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("B_KNIGHT", "b_knight.png") != nullptr);
	}

	{
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("W_PAWN", "w_pawn.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("W_KING", "w_king.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("W_QUEEN", "w_queen.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("W_ROOK", "w_rook.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("W_BISHOP", "w_bishop.png") != nullptr);
		ASSERT_CRASH(GET_SINGLE(ResourceManager)->LoadTexture("W_KNIGHT", "w_knight.png") != nullptr);
	}
	
}

void DevScene::LoadUIs()
{
	// Attack Zone Ã¼Å©
	{
		Actor* actor = new BoxUI(G_SHOW_ATTACK_ZONE_LEFT_TOP, G_SHOW_ATTACK_ZONE_RIGHT_BOTTOM, 
			G_SHOW_ATTACK_ZONE_LEFT_TOP + VectorInt(10, 23), L"TOGGLE SHOW ATTACK ZONE");
		AddActor(actor);
		UI* ui = static_cast<UI*>(actor);
		ui->SetFunc(std::bind(&Board::ToggleShowAttackZone, _board));
		
	}
	// CanGo Toggle Box
	{
		Actor* actor = new BoxUI(G_CAN_GO_TOGGLE_BOX_LEFT_TOP, G_CAN_GO_TOGGLE_BOX_RIGHT_BOTTOM, 
			G_CAN_GO_TOGGLE_BOX_LEFT_TOP + VectorInt(32, 23), L"TOGGLE SHOW CAN GO");
		AddActor(actor);
		UI* ui = static_cast<UI*>(actor);
		ui->SetFunc(std::bind(&Board::ToggleShowCanGo, _board));
	}
	// Move Back
	{
		Actor* actor = new BoxUI(G_MOVE_BACK_BOX_LEFT_TOP, G_MOVE_BACK_BOX_RIGHT_BOTTOM, 
			G_MOVE_BACK_BOX_LEFT_TOP + VectorInt(82, 23), L"MOVE BACK");
		AddActor(actor);
		UI* ui = static_cast<UI*>(actor);
		ui->SetFunc(std::bind(&Board::MoveBack, _board));
	}
}

void DevScene::GoNextTurn()
{
	_currTurn = _currTurn == PLAYER_TURN ? AI_TURN : PLAYER_TURN;
	++_currTurnNo;


	if (_board->IsKingInCheckMate(GetCurrTurnPlayersColor()))
	{
		GET_SINGLE(ResourceManager)->PlaySounds("CheckMate");
		_isCheckMate = true;
	}

	if (_currTurn == AI_TURN)
		_ai->ResetMove();

	if (_currTurn == PLAYER_TURN)
	{
		_playerTurnStartTime = ::GetTickCount64();
		_player->ResetMove();
	}
}

void DevScene::GoBackTurn()
{
	_currTurn = PLAYER_TURN;
	_currTurnNo -= 2;
}

ColorType DevScene::GetCurrTurnPlayersColor() const
{
	if (_currTurn == PLAYER_TURN)
		return GetPlayerColor() == ColorType::Black ? ColorType::Black : ColorType::White;
	else
		return GetPlayerColor() == ColorType::Black ? ColorType::White : ColorType::Black;
	
}


TrashTray* DevScene::GetTrashTray(PlayerType type) const
{
	return _trashes[static_cast<int32>(type)];
}

void DevScene::CheckMate()
{
	uint64 prev = ::GetTickCount64();

	GET_SINGLE(ResourceManager)->PlaySounds("Capture");

	_isCheckMate = true;
}

bool DevScene::IsGameOver()
{
	bool isKingCaptured = _board->IsKingCaptured();
	/*if (isKingCaptured && _currTurn == PLAYER_TURN)
	{
		GET_SINGLE(ResourceManager)->PlaySounds("Lose");
	}*/

	return isKingCaptured;
}

void DevScene::GiveUp()
{
	//GET_SINGLE(ResourceManager)->PlaySounds("Win");
	_aiGiveup = true;
}

void DevScene::ResetGame()
{
}



