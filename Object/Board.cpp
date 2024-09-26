#include "pch.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Board.h"
#include "Texture.h"
#include "DevScene.h"
#include "Player.h"
#include "Human.h"
#include "AI.h"

#include "Pawn.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "TrashTray.h"

Board::Board(Texture* texture, ColorType playerColor)
	: Super(LAYER_OBJECT)
	, _playerColor(playerColor)
{
	SetTexture(texture);

	_board = new Piece*[G_TILE_COUNT * G_TILE_COUNT]();    // nullptr 초기화
	

	SetPos({ G_BOARD_POSX, G_BOARD_POSY });
	for (int y = 0; y < G_TILE_COUNT; ++y)
	{
		for (int x = 0; x < G_TILE_COUNT; ++x)
		{
			_board[y * G_TILE_COUNT + x] = nullptr;
		}
	}

	SetFactory();
	_aiPieces.reserve(16);
	_playerPieces.reserve(16);
	
	_blendFunction.BlendOp = AC_SRC_OVER;       // 알파 채널을 사용해 이미지를 그립니다.
	_blendFunction.BlendFlags = 0;              // 사용되지 않으므로 0으로 설정합니다.
	_blendFunction.SourceConstantAlpha = 100;   // 이미지의 투명도. (0: 완전 투명, 255: 완전 불투명)
	_blendFunction.AlphaFormat = AC_SRC_ALPHA;  // 소스 이미지는 알파 채널을 포함하고 있습니다.
	
	_cangoTexture = GET_SINGLE(ResourceManager)->LoadTexture("CangoTexture", "square_can_go.png");
	ASSERT_CRASH(_cangoTexture != nullptr);
	_colorBoxes[1] = GET_SINGLE(ResourceManager)->LoadTexture("RedBox", "redSquare.png");
	_colorBoxes[2] = GET_SINGLE(ResourceManager)->LoadTexture("OrangeBox", "orangeSquare.png");
	_colorBoxes[3] = GET_SINGLE(ResourceManager)->LoadTexture("YellowBox", "yellowSquare.png");
	for (int32 i = 1; i < 4; ++i)
		ASSERT_CRASH(_colorBoxes[i] != nullptr);

	_checkMateTex = GET_SINGLE(ResourceManager)->LoadTexture("CHECK_MATE", "checkMate.png");
	ASSERT_CRASH(_checkMateTex != nullptr);

	_aiMoveFrom = GET_SINGLE(ResourceManager)->LoadTexture("AI_MOVE_FROM", "aiMoveFrom.png");
	_aiMoveTo = GET_SINGLE(ResourceManager)->LoadTexture("AI_MOVE_TO", "aiMoveTo.png");
	ASSERT_CRASH(_aiMoveFrom != nullptr);
	ASSERT_CRASH(_aiMoveTo != nullptr);
	
}

Board::~Board()
{
	DeleteAll();
	delete[] _board;
	_board = nullptr;
	
}

void Board::DeleteAll()
{
	for (int y = 0; y < G_TILE_COUNT; ++y)
	{
		for (int x = 0; x < G_TILE_COUNT; ++x)
		{
			SAFE_DELETE(_board[y * G_TILE_COUNT + x]);
		}
	}
	if (_aiTray)
		_aiTray->DeleteObjects();
	if (_playerTray)
		_playerTray->DeleteObjects();
}

void Board::Init()
{
}

void Board::Update()
{
	Turn currTurn = GET_DEVSCENE->GetCurrTurn();
	Player * currPlayer = currTurn == PLAYER_TURN ? _player : _ai;

	if (_isPromotion)
	{
		Piece* targetPiece = nullptr;
		targetPiece = currTurn == PLAYER_TURN ? 
			_playerTray->PickPieceForPromotionOrNull() :
			_aiTray->GetPromotionPieceForAi();

		if (targetPiece == nullptr)
			return;

		DoPromotion(targetPiece);

		_promotePawn = nullptr;
		_isPromotion = false;

		GET_DEVSCENE->GoNextTurn();
		return;
	}
	
	currPlayer->Update();

	const Move lastMove = currPlayer->GetLastMove();

	if (lastMove.IsEmpty())
		return;

	Piece* movedPiece = GetPieceOnIdxOrNull(lastMove.from);

	UpdatePiecePos(lastMove);

	// promotion 체크
	if (movedPiece->GetType() == TYPE_PAWN)
	{
		if (ShouldPromote(movedPiece))
		{
			_isPromotion = true;
			_promotePawn = movedPiece;
			return;
		}
	}

	UpdateAttackZone(GET_DEVSCENE->GetCurrTurn() == PLAYER_TURN);
	GET_DEVSCENE->GoNextTurn();
	
	
}

void Board::Render(HDC hdc)
{
	Super::Render(hdc);

	for (int y = 0; y < G_TILE_COUNT; ++y)
	{
		for (int x = 0; x < G_TILE_COUNT; ++x)
		{
			if (_board[y * G_TILE_COUNT + x] != nullptr)
				_board[y * G_TILE_COUNT + x]->Render(hdc);
		}
	}
	
	if (_cangoMarkOn && _availableMoves != nullptr)
	{
		_blendFunction.SourceConstantAlpha = 40;

		for (const Move& m : *_availableMoves)
		{
			int32 sizeX = _cangoTexture->GetSize().x;
			int32 sizeY = _cangoTexture->GetSize().y;

			int32 startX = G_BOARD_STARTX + m.to.x * G_TILE_SIZE;
			int32 startY = G_BOARD_STARTY + m.to.y * G_TILE_SIZE;

			AlphaBlend(hdc, startX, startY,
				sizeX, sizeY, _cangoTexture->GetDC(), 0, 0, sizeX, sizeY, _blendFunction);
		}
	}

	if (_showAttackZone && GET_DEVSCENE->GetCurrTurn() == PLAYER_TURN)
	{
		Texture* texture = nullptr;
		_blendFunction.SourceConstantAlpha = 60;

		for (int32 i = 0; i < 64; ++i)
		{
			if (_aisAttackZone[i] == Z_None)
				continue;

			int32 boxIdx = static_cast<int32>(_aisAttackZone[i]);
			texture = _colorBoxes[boxIdx];

			int32 sizeX = texture->GetSize().x;
			int32 sizeY = texture->GetSize().y;

			int32 x = G_BOARD_STARTX + i % G_TILE_COUNT * G_TILE_SIZE;
			int32 y = G_BOARD_STARTY + i / G_TILE_COUNT * G_TILE_SIZE;

			AlphaBlend(hdc, x, y,
				sizeX, sizeY, texture->GetDC(), 0, 0, sizeX, sizeY, _blendFunction);
		}
	}

	if (GET_DEVSCENE->GetCurrTurn() == PLAYER_TURN && GET_DEVSCENE->GetCurrTurnNo() >= 1)
	{
		uint64 now = ::GetTickCount64();
		uint64 playerTurnStartTime = GET_DEVSCENE->GetPlayerTurnStartTime();
		
		if (now - playerTurnStartTime < 2000)
		{
			_blendFunction.SourceConstantAlpha = 230;

			const Move aiLastMove = _ai->GetLastMove();
			const VectorInt& from = aiLastMove.from;
			const VectorInt& to = aiLastMove.to;

			int32 fromX = G_BOARD_STARTX + from.x * G_TILE_SIZE;
			int32 fromY = G_BOARD_STARTY + from.y * G_TILE_SIZE;
			int32 toX = G_BOARD_STARTX + to.x * G_TILE_SIZE;
			int32 toY = G_BOARD_STARTY + to.y * G_TILE_SIZE;

			AlphaBlend(hdc, fromX, fromY, G_TILE_SIZE, G_TILE_SIZE,
				_aiMoveFrom->GetDC(), 0, 0, G_TILE_SIZE, G_TILE_SIZE, _blendFunction);

			AlphaBlend(hdc, toX, toY, G_TILE_SIZE, G_TILE_SIZE,
				_aiMoveTo->GetDC(), 0, 0, G_TILE_SIZE, G_TILE_SIZE, _blendFunction);
		}
	}
}

VectorInt Board::ChangePosToIdx(const VectorInt& pos)
{
	if (!IsInBoard(pos))
	{
		return VectorInt(-1, -1);
	}

	return VectorInt((pos.x - G_BOARD_STARTX) / G_TILE_SIZE, (pos.y - G_BOARD_STARTY) / G_TILE_SIZE);
}

void Board::UpdatePiecePos(const Move& move)
{
	if (!IsValidIdx(move.from) || move.from == move.to)
		ASSERT_CRASH(false);

	if (!IsKingInCheckMate(GET_DEVSCENE->GetCurrTurnPlayersColor()))
		_isCheckMate = false;

	if (move.captured && move.captured->GetType() == TYPE_KING)
		_isKingCaptured = true;

	_history.push(move);

	if (move.captured)
		GET_SINGLE(ResourceManager)->PlaySounds("Capture");
	else if (GET_DEVSCENE->GetCurrTurn() == PLAYER_TURN)
	{
		GET_SINGLE(ResourceManager)->PlaySounds("Move2");
	}
	else
		GET_SINGLE(ResourceManager)->PlaySounds("Move2");

	if (move.captured)
	{
		move.captured->SetPosIdx({ -1, -1 });
		RemovePieceFromEachPlayer(move.captured, move.captured->IsPlayers());
		MoveToTrash(move.captured, move.captured->IsPlayers());
	}

	Piece* movingPiece = GetPieceOnIdxOrNull(move.from);
	ASSERT_CRASH(movingPiece != nullptr);
	
	movingPiece->SetPosIdx(move.to);

	_board[move.to.y * G_TILE_COUNT + move.to.x] = movingPiece;
	_board[move.from.y * G_TILE_COUNT + move.from.x] = nullptr;
	
}

void Board::MoveBack()
{
	if (_history.empty() || GET_DEVSCENE->GetCurrTurn() == AI_TURN)
		return;

	ASSERT_CRASH(_history.size() >= 2);

	Move aiMove = _history.top();
	_history.pop();
	Move playerMove = _history.top();
	_history.pop();

	MakeMoveBackOnce(aiMove);
	MakeMoveBackOnce(playerMove);

	GET_DEVSCENE->GoBackTurn();
}

void Board::DoPromotion(Piece* targetPiece)
{
	ASSERT_CRASH(_promotePawn != nullptr);

	TrashTray* tray = _promotePawn->IsPlayers() ? _playerTray : _aiTray;
	
	targetPiece->SetPosIdx(_promotePawn->GetPosIdx());
	RemovePieceFromEachPlayer(_promotePawn, _promotePawn->IsPlayers());
	MoveToTrash(_promotePawn, _promotePawn->IsPlayers(), true);
	vector<Piece*>& pieceInHand = _promotePawn->IsPlayers() ? _playerPieces : _aiPieces;
	pieceInHand.push_back(targetPiece);
	
	VectorInt boardPos = targetPiece->GetPosIdx();
	_board[VectorToIdx(boardPos)] = nullptr;
	
	AddPieceToBoard(boardPos, targetPiece);

	GET_SINGLE(ResourceManager)->PlaySounds("Promotion");

}



void Board::MakeMove(const Move& move)
{
	ASSERT_CRASH(IsValidIdx(move.from) && IsValidIdx(move.to));
	Piece* movingPiece = GetPieceOnIdxOrNull(move.from);
	movingPiece->SetPosIdx(move.to);
	_board[move.to.y * G_TILE_COUNT + move.to.x] = movingPiece;
	_board[move.from.y * G_TILE_COUNT + move.from.x] = nullptr;
	if (move.captured)
		RemovePieceFromEachPlayer(move.captured, move.captured->IsPlayers());
	
}

void Board::UnMove(const Move& move)
{
	ASSERT_CRASH(IsValidIdx(move.from) && IsValidIdx(move.to));
	
	Piece* movedPiece = GetPieceOnIdxOrNull(move.to);
	movedPiece->SetPosIdx(move.from);
	Piece* originalPiece = move.captured;

	_board[move.from.y * G_TILE_COUNT + move.from.x] = movedPiece;
	_board[move.to.y * G_TILE_COUNT + move.to.x] = originalPiece;

	if (!originalPiece)
		return;

	originalPiece->SetPosIdx(move.to);

	if (move.captured->IsPlayers())
		_playerPieces.push_back(move.captured);
	else
		_aiPieces.push_back(move.captured);
}

const Zone* Board::GetEnemysAttackZone(bool IsForPlayer)
{
	return IsForPlayer ? _aisAttackZone : _playersAttackZone;
}

const Zone Board::GetZoneColor(bool isForPlayer, const VectorInt& pos)
{
	int32 idx = VectorToIdx(pos);
	
	return isForPlayer ? _aisAttackZone[idx] : _playersAttackZone[idx];
}


void Board::UpdateAttackZone(bool players)
{
	Zone* zone = players ? _playersAttackZone : _aisAttackZone;
	vector<Piece*> piecesInHand = players ? _playerPieces : _aiPieces;
	::memset(zone, Z_None, sizeof(Zone) * 64);

	for (Piece* p : piecesInHand)
		p->GetAttackZone(zone);
}


bool Board::IsKingCaptured() const
{
	return _isKingCaptured;
}

bool Board::IsKingInCheckMate(ColorType color)
{
	Turn currTurn = GET_DEVSCENE->GetCurrTurn();
	
	const Piece* king = color == _kings[0]->GetPieceColor() ? _kings[0] : _kings[1];
	const VectorInt kingPos = king->GetPosIdx();
	Zone kingPosAttackZoneColor = GetZoneColor(king->IsPlayers(), kingPos);

	if (kingPosAttackZoneColor == Z_Red || kingPosAttackZoneColor == Z_Orange)
		return true;

	return false;
}

const vector<Piece*>& Board::GetMyPieces(PlayerType playerType)
{
	return playerType == PlayerType::Human ? _playerPieces : _aiPieces;
}

Piece* Board::GetPieceOnIdxOrNull(const VectorInt& posIdx)
{
	if (!IsValidIdx(posIdx))
		return nullptr;
	return _board[posIdx.y * G_TILE_COUNT + posIdx.x];
}

void Board::SetPlayer(Player* player)
{
	if (player->GetType() == PlayerType::Human)
		_player = player;
	else
		_ai = player;
}

void Board::SetTrashTray(TrashTray* tray, PlayerType playerType)
{
	if (playerType == PlayerType::Human)
		_playerTray = tray;
	else
		_aiTray = tray;
}

inline bool Board::IsInBoard(const VectorInt& pos)
{
	if (pos.x < G_BOARD_STARTX || pos.y < G_BOARD_STARTY)
		return false;
	if (pos.x > G_BOARD_STARTX + G_BOARD_LENGTH || pos.y > G_BOARD_STARTY + G_BOARD_LENGTH)
		return false;
	return true;
}

inline bool Board::IsValidIdx(const VectorInt& posIdx)
{
	if (posIdx.x < 0 || posIdx.y < 0 || posIdx.x >= G_TILE_COUNT || posIdx.y >= G_TILE_COUNT)
		return false;
	return true;
}

void Board::RemovePieceFromEachPlayer(Piece* p, bool isPlayers)
{
	if (isPlayers)
	{
		auto findIt = std::find(_playerPieces.begin(), _playerPieces.end(), p);
		if (findIt != _playerPieces.end())
			_playerPieces.erase(findIt);
	}
	else
	{
		auto findIt = std::find(_aiPieces.begin(), _aiPieces.end(), p);
		if (findIt != _aiPieces.end())
			_aiPieces.erase(findIt);
	}
}


void Board::MoveToTrash(Piece* p, bool isPlayers, bool isPromotedPawn)
{
	p->SetPosIdx({ -1, -1 });

	if (isPlayers)
		_playerTray->AddTrash(p, isPromotedPawn);
	else
		_aiTray->AddTrash(p, isPromotedPawn);
}

void Board::MakeMoveBackOnce(const Move& move)
{
	int32 moveBackFrom = move.to.y * G_TILE_COUNT + move.to.x;
	int32 moveBackTo = move.from.y * G_TILE_COUNT + move.from.x;

	Piece* lastMovedPiece = _board[moveBackFrom];
	_board[moveBackFrom] = move.captured;

	if (move.captured)
	{
		if (move.captured->IsPlayers())
		{
			_playerPieces.push_back(move.captured);
			_playerTray->RemovePieceInTray(move.captured);
		}
		else
		{
			_aiPieces.push_back(move.captured);
			_aiTray->RemovePieceInTray(move.captured);
		}
		
		move.captured->SetPosIdx(move.to);
	}

	ASSERT_CRASH(lastMovedPiece != nullptr);

	lastMovedPiece->SetPosIdx(move.from);

	_board[moveBackTo] = lastMovedPiece;
}


bool Board::ShouldPromote(const Piece* piece)
{
	if (piece->IsPlayers())
		return piece->GetPosIdx().y == 0 && !_playerTray->IsEmpty() ? true : false;
	else
		return piece->GetPosIdx().y == 7 && !_aiTray->IsEmpty() ? true : false;
}



// For Debug
void Board::SetBoard(PieceChar* board, ColorType playerColor)
{
	DeleteAll();

	_playerColor = playerColor;
	
	for (int32 y = 0; y < G_TILE_COUNT; ++y)
	{
		for (int32 x = 0; x < G_TILE_COUNT; ++x)
		{
			// 0~5:Black  6~11:White
			uint8 piece = board[y * G_TILE_COUNT + x];

			if (piece == XX)
			{
				_board[y * G_TILE_COUNT + x] = nullptr;
			}
			else if (piece <= 5)
			{
				Piece* p = _pieceFactory[piece](ColorType::Black);
				p->SetPosIdx({ x, y });
				_board[y * G_TILE_COUNT + x] = p;

				if (playerColor == ColorType::Black)
					_playerPieces.push_back(p);
				else
					_aiPieces.push_back(p);
			}
			else
			{
				piece -= 6;
				Piece* p = _pieceFactory[piece](ColorType::White);
				p->SetPosIdx({ x, y });
				_board[y * G_TILE_COUNT + x] = p;

				if (playerColor == ColorType::White)
					_playerPieces.push_back(p);
				else
					_aiPieces.push_back(p);
			}
		}
	}

	for (Piece* p : _playerPieces)
		UpdateAttackZone(true);
	for (Piece* p : _aiPieces)
		UpdateAttackZone(false);
}

void Board::CreateTrashPiece(PlayerType playerType, ColorType playerColor, PieceType pieceType)
{
	Piece* piece = _pieceFactory[pieceType](playerColor);
	if (playerType == PlayerType::Human)
		_playerTray->AddTrash(piece);
	else
		_aiTray->AddTrash(piece);
}

void Board::ResetGame(ColorType colorChoice)
{
	// Todo
}

void Board::SetFactory()
{
	ResourceManager* rm = GET_SINGLE(ResourceManager);

	// pieceFactory
	_pieceFactory[0] = [=](ColorType color) {
		Piece* p = new King(this, color);
		if (color == ColorType::Black)
			p->SetTexture(rm->GetTexture("B_KING"));
		else
			p->SetTexture(rm->GetTexture("W_KING"));

		if (color == _playerColor)
			_kings[0] = p;
		else
			_kings[1] = p;

		return p; };
	
	_pieceFactory[1] = [=](ColorType color) {
		Piece* p = new Queen(this, color);
		if (color == ColorType::Black)
			p->SetTexture(rm->GetTexture("B_QUEEN"));
		else
			p->SetTexture(rm->GetTexture("W_QUEEN"));
		return p; };

	_pieceFactory[2] = [=](ColorType color) {
		Piece* p = new Rook(this, color);
		if (color == ColorType::Black)
			p->SetTexture(rm->GetTexture("B_ROOK"));
		else
			p->SetTexture(rm->GetTexture("W_ROOK"));
		return p; };

	_pieceFactory[3] = [=](ColorType color) {
		Piece* p = new Bishop(this, color);
		if (color == ColorType::Black)
			p->SetTexture(rm->GetTexture("B_BISHOP"));
		else
			p->SetTexture(rm->GetTexture("W_BISHOP"));
		return p; };

	_pieceFactory[4] = [=](ColorType color) {
		Piece* p = new Knight(this, color);
		if (color == ColorType::Black)
			p->SetTexture(rm->GetTexture("B_KNIGHT"));
		else
			p->SetTexture(rm->GetTexture("W_KNIGHT"));
		return p; };

	_pieceFactory[5] = [=](ColorType color) {
		Piece* p = new Pawn(this, color);
		if (color == ColorType::Black)
			p->SetTexture(rm->GetTexture("B_PAWN"));
		else
			p->SetTexture(rm->GetTexture("W_PAWN"));
		return p; };
}

void Board::AddPieceToBoard(const VectorInt& pos, Piece* p)
{
	int32 idx = VectorToIdx(pos);
	ASSERT_CRASH(_board[idx] == nullptr);
	_board[idx] = p;
}


// TEMP LOG

string Board::GetBoardString()
{
	string str = "";
	str.reserve(200);

	for (int32 y = 0; y < G_TILE_COUNT; ++y)
	{
		for (int32 x = 0; x < G_TILE_COUNT; ++x)
		{
			Piece* p = _board[y * G_TILE_COUNT + x];
			if (p == nullptr)
			{
				str += "00 ";
				continue;
			}

			switch (p->GetType())
			{
			case PieceType::TYPE_KING:
				if (p->GetPieceColor() == ColorType::Black)
					str += "BK ";
				else
					str += "WK ";
				break;
			case PieceType::TYPE_QUEEN:
				if (p->GetPieceColor() == ColorType::Black)
					str += "BQ ";
				else
					str += "WQ ";
				break;
			case PieceType::TYPE_ROOK:
				if (p->GetPieceColor() == ColorType::Black)
					str += "BR ";
				else
					str += "WR ";
				break;
			case PieceType::TYPE_BISHOP:
				if (p->GetPieceColor() == ColorType::Black)
					str += "BB ";
				else
					str += "WB ";
				break;
			case PieceType::TYPE_KNIGHT:
				if (p->GetPieceColor() == ColorType::Black)
					str += "BN ";
				else
					str += "WN ";
				break;
			case PieceType::TYPE_PAWN:
				if (p->GetPieceColor() == ColorType::Black)
					str += "BP ";
				else
					str += "WP ";
				break;
			}
		}
		str += "\n";
	}

	return str;
}

string Board::GetAttackZoneString(bool isForPlayer)
{
	Zone* zone = isForPlayer ? _aisAttackZone : _playersAttackZone;

	string str = "";
	str.reserve(200);

	for (int32 y = 0; y < 8; ++y)
	{
		for (int32 x = 0; x < 8; ++x)
		{
			int32 idx = y * 8 + x;
			if (zone[idx] == Z_None)
				str += "OO ";
			else if (zone[idx] == Z_Red)
				str += "RE ";
			else if (zone[idx] == Z_Orange)
				str += "OR ";
			else
				str += "YE ";

		}
		str += "\n";
	}

	return str;
}

string Board::GetAttackZoneString(Zone zone)
{
	switch (zone)
	{
	case Z_None:
		return "None";
	case Z_Red:
		return "Red";
	case Z_Orange:
		return "Orange";
	case Z_Yellow:
		return "Yellow";
	default:
		ASSERT_CRASH(false);
		return "NULL";
	}
}
string Board::GetPieceTypeString(Piece* piece)
{
	if (piece == nullptr)
		return "NULL";

	PieceType type = piece->GetType();
	
	switch(type)
	{
	case TYPE_BISHOP:
		return "Bishop";
	case TYPE_KING:
		return "King";
	case TYPE_QUEEN:
		return "Queen";
	case TYPE_ROOK:
		return "Rook";
	case TYPE_KNIGHT:
		return "Knight";
	case TYPE_PAWN:
		return "Pawn";
	default:
		ASSERT_CRASH(false);
		return "";
	}

	return "";
}