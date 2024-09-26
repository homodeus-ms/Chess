#pragma once
#include "Actor.h"
#include "Piece.h"

class Texture;
class Mover;
class TrashTray;
class Player;

class Board : public Actor
{
	using Super = Actor;
public:
	Board(Texture* texture, ColorType color);
	virtual ~Board();
	void DeleteAll();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	
	Piece** GetBoard() { return _board; }
	Piece* GetPieceOnIdxOrNull(const VectorInt& posIdx);
	void SetPlayer(Player* player);
	void SetAvailableMoves(const vector<Move>* moves) { _availableMoves = moves; }
	void SetTrashTray(TrashTray* tray, PlayerType playerType);

	inline bool IsInBoard(const VectorInt& pos);
	inline bool IsValidIdx(const VectorInt& posIdx);
	inline bool IsPromotion() const { return _isPromotion; }
	VectorInt ChangePosToIdx(const VectorInt& pos);
	

	void UpdatePiecePos(const Move& move);
	void MakeMove(const Move& move);
	void UnMove(const Move& move);

	// AttackZone 관련
	const Zone* GetEnemysAttackZone(bool IsForPlayer);
	const Zone GetZoneColor(bool isForPlayer, const VectorInt& pos);
	void UpdateAttackZone(bool players);
	
	bool IsKingCaptured() const;
	bool IsKingInCheckMate(ColorType color);
	VectorInt GetKingPos(PlayerType playerType) const { return _kings[static_cast<int32>(playerType)]->GetPosIdx(); }
	Piece* GetKingPtr(PlayerType playerType) const { return _kings[static_cast<int32>(playerType)]; }

	const vector<Piece*>& GetMyPieces(PlayerType playerType);
	int32 GetCurrScore() const { return _aiScore - _playerScore; }

	//  UI 클래스에 함수포인터로 넘겨줄 함수들
	void MoveBack();
	void ToggleShowCanGo() { _cangoMarkOn ^= true; }
	void ToggleShowAttackZone() { _showAttackZone ^= true; }
	

	// 추가 기능
	void DoPromotion(Piece* targetPiece);

	// 기타 Helper함수
	inline int32 VectorToIdx(const VectorInt& v) const { return v.y * G_TILE_COUNT + v.x; }
	
private:
	void SetFactory();
	void AddPieceToBoard(const VectorInt& pos, Piece* p);
	void RemovePieceFromEachPlayer(Piece* p, bool isPlayers);
	void MoveToTrash(Piece* p, bool isPlayers, bool isPromotedPawn = false);
	void MakeMoveBackOnce(const Move& move);
	
	
	bool ShouldPromote(const Piece* pawn);


	// For Debug
public:
	// TEMP LOG
	string GetBoardString();
	string GetAttackZoneString(bool isForPlayer);
	string GetAttackZoneString(Zone zone);
	string GetPieceTypeString(Piece* piece);

	void SetBoard(PieceChar* board, ColorType playerColor);
	void CreateTrashPiece(PlayerType playerType, ColorType playerColor, PieceType pieceType);
	void ResetGame(ColorType colorChoice);
	
private:
	
	// king, queen, rook, bishop, knight, pawn
	function<Piece* (ColorType color)> _pieceFactory[6];

	Player* _player = nullptr;
	Player* _ai = nullptr;

	vector<Piece*> _aiPieces;
	vector<Piece*> _playerPieces;
	TrashTray* _aiTray = nullptr;
	TrashTray* _playerTray = nullptr;

	Piece** _board = nullptr;
	ColorType _playerColor;

	// 0 = Player's King, 1 = Ai'sKing
	Piece* _kings[2];
	const vector<Move>* _availableMoves = nullptr;
	Zone _playersAttackZone[64] = { Z_None, };
	Zone _aisAttackZone[64] = { Z_None, };


	Texture* _cangoTexture = nullptr;
	// [1] = Red, [2] = Orange, [3] = Yellow
	Texture* _colorBoxes[4] = { nullptr, };
	Texture* _checkMateTex = nullptr;
	Texture* _aiMoveFrom = nullptr;
	Texture* _aiMoveTo = nullptr;
	BLENDFUNCTION _blendFunction = {};

	bool _cangoMarkOn = true;
	bool _showAttackZone = false;
	bool _isCheckMate = false;
	bool _isKingCaptured = false;
	bool _isPromotion = false;
	Piece* _promotePawn = nullptr;
	
	int32 _aiScore = 5000;
	int32 _playerScore = 5000;

	// MoveBack관련
	stack<Move> _history;
	

};



