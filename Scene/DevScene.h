#pragma once
#include "GameScene.h"
#include "LogClass.h"

class Board;
class Player;
class AI;
class TrashTray;


class DevScene : public GameScene
{
	using Super = GameScene;

public:
	DevScene();
	virtual ~DevScene();

	void Init() override;
	void Update() override;
	void Render(HDC hdc) override;

	void LoadImages();
	void LoadUIs();

	void GoNextTurn();
	void GoBackTurn();

	Turn GetCurrTurn() const { return _currTurn; }
	ColorType GetCurrTurnPlayersColor() const;
	
	uint32 GetCurrTurnNo() const { return _currTurnNo; }
	uint64 GetPlayerTurnStartTime() const { return _playerTurnStartTime; }
	Board* GetBoard() const { return _board; }
	

	TrashTray* GetTrashTray(PlayerType type) const;

	void CheckMate();
	bool IsGameOver();
	void GiveUp();
	void ResetGame();

private:
	
	Player* _player;
	Player* _ai;

	Turn _currTurn = PLAYER_TURN;
	uint32 _currTurnNo = 0;
	uint64 _playerTurnStartTime = 0;
	
	TrashTray* _trashes[2] = { nullptr, };
	Board* _board = nullptr;

	Actor* _checkMate = nullptr;
	bool _isCheckMate = false;
	bool _aiGiveup = false;
	bool _endingSoundPlayed = false;

	// TEMP LOG
public:
	LogClass& GetLogClass() { return _logClass; }

private:

	LogClass _logClass;

#define XX (NP)

	PieceChar TEMP_CAPTURE[64] = { XX, };

	PieceChar PLAYER_BLACK_BASIC_FORM[64] = {
		 WR, WN, WB, WQ, WK, WB, WN, WR,
		 WP, WP, WP, WP, WP, WP, WP, WP,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 BP, BP, BP, BP, BP, BP, BP, BP,
		 BR, BN, BB, BQ, BK, BB, BN, BR,
	};
	PieceChar PLAYER_WHITE_BASIC_FORM[64] = {
		 
		 BR, BN, BB, BQ, BK, BB, BN, BR,
		 BP, BP, BP, BP, BP, BP, BP, BP,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 WP, WP, WP, WP, WP, WP, WP, WP,
		 WR, WN, WB, WQ, WK, WB, WN, WR,
		 
	};


	// Test¿ë
	PieceChar PLAYER_BLACK_TEST1[64] = {
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, BP, BK, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, WK, XX, XX, XX,
		 XX, XX, BR, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, BQ, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
	};
	PieceChar PLAYER_BLACK_TEST2[64] = {
		 XX, XX, WB, XX, WK, XX, XX, WR,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, XX, WR, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, XX, XX, XX,
		 XX, XX, BP, XX, BP, XX, XX, XX,
		 XX, XX, BN, XX, XX, XX, XX, XX,
		 XX, XX, XX, XX, XX, BQ, XX, XX,
		 BR, XX, XX, XX, BK, XX, XX, XX,
	};

	PieceChar BASIC_TEST1[64] = {
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, WB, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, BP, XX, XX,
		XX, XX, XX, XX, XX, XX, BP, XX,
		XX, BN, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
	};

	PieceChar BASIC_TEST2[64] = {
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, WB, WK, WN, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		BR, XX, XX, XX, XX, XX, XX, BR,
		XX, XX, XX, XX, XX, BQ, XX, XX,
		XX, BK, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
	};

	PieceChar GIVEUP_TEST2[64] = {
		XX, WK, XX, XX, XX, XX, XX, XX,
		XX, WP, WR, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, BQ, XX, XX,
		BN, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, BR, XX, XX, XX, XX,
		XX, BK, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
	};
	PieceChar PROMOTION_TEST[64] = {
		XX, XX, XX, XX, XX, XX, XX, XX,
		BP, BP, XX, XX, WK, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		BN, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, WQ, XX, XX, XX,
		XX, BK, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
	};

	PieceChar TEST1[64] = {
		XX, XX, WP, WK, WP, XX, XX, XX,
		BP, XX, WP, XX, WP, XX, BP, BP,
		XX, XX, XX, XX, XX, WQ, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		BR, XX, XX, XX, XX, BR, XX, XX,
		XX, XX, XX, BK, XX, XX, XX, XX,
	};
	PieceChar TEST2[64] = {
		XX, XX, XX, XX, WP, WK, WN, WR,
		XX, XX, XX, XX, WN, XX, WP, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, BP,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, BR, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, BQ, XX,
		XX, XX, XX, BK, XX, XX, XX, BR,
	};
	PieceChar TEST3[64] = {
		XX, XX, XX, XX, XX, XX, XX, WK,
		XX, XX, XX, XX, XX, BR, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, BK, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
	};
	PieceChar TEST2_WHITE[64] = {
		BR, XX, XX, XX, BP, BK, BN, BR,
		XX, XX, XX, XX, BP, XX, BP, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		WB, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, WP,
		XX, XX, XX, XX, WR, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, WQ, XX,
		XX, XX, XX, WK, XX, XX, XX, WR,
	};
	PieceChar TEST4[64] = {
		WR, XX, WB, XX, WK, WB, XX, WR,
		WP, WP, XX, XX, XX, XX, WP, WP,
		XX, XX, WQ, XX, XX, XX, XX, XX,
		XX, BR, BP, XX, BP, XX, XX, XX,
		BP, XX, XX, BQ, WP, XX, XX, BP,
		XX, XX, XX, XX, BP, BR, BP, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, BB, XX, BK, BB, BN, BR,
	};
	PieceChar TEST5[64] = {
		XX, XX, XX, WK, XX, XX, XX, XX,
		XX, XX, XX, WQ, WN, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		BN, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, XX, XX, XX, XX, XX,
		XX, BK, XX, XX, XX, XX, XX, XX,
		XX, XX, XX, BQ, XX, XX, BR, XX,
		
	};
};

