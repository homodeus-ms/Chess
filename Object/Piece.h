#pragma once
#include "Actor.h"

class Board;

struct Move
{
	Move() {}
	Move(const VectorInt& fromIdx, VectorInt& toIdx, Piece* captured = nullptr) : from(fromIdx), to(toIdx), captured(captured) {}
	VectorInt from = { -1, -1 };
	VectorInt to = { -1, -1 };
	Piece* captured = nullptr;

	void Reset()
	{
		from = { -1, -1 };
		to = { -1, -1 };
		captured = nullptr;
	}
	bool IsEmpty() const
	{
		return from.x == -1 && from.y == -1;
	}
};


class Piece : public Actor
{
	using Super = Actor;
public:
	Piece(Board* board, PieceType type, ColorType color);
	virtual ~Piece();
	Piece(const Piece& other) = delete;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual bool CanMove(VectorInt fromIdx, VectorInt toIdx) const;
	virtual void GetAvailableMoves(OUT vector<Move>& moves) abstract;
	virtual void GetAttackZone(OUT Zone* zone) abstract;

	VectorInt GetPosIdx() const { return _posIdx; }
	void SetPosIdx(const VectorInt& pos);
	PieceType GetType() const { return _pieceType; }
	ColorType GetPieceColor() const { return _color; }
	bool IsPlayers() const;
	

protected:
	void GetAvailableMoves(OUT vector<Move>& moves, const VectorInt* offset, int32 offsetLength);
	void CalculateAttackZone(const VectorInt* offset, int32 offsetLength, OUT Zone* zone);
	bool IsLegalMove(const Piece* movingPiece, const VectorInt& from, const VectorInt& to);

	
private:
	bool IsKingBehind(const VectorInt& from, const VectorInt& offset, ColorType targetColor);
	void FillColor(const vector<int32>& indices, Zone zoneColor, OUT Zone* zone);
	
	

public:
	static const VectorInt* KNIGHT_OFFSET;
	static const VectorInt* COMPOSITE_OFFSET;
	static const VectorInt* STRAIGHT_OFFSET;
	static const VectorInt* DIAGONAL_OFFSET;
	static const VectorInt* PAWN_OFFSET;

protected:

	
	PieceType _pieceType;
	Board* _board;
	Piece** _boardState;
	VectorInt _posIdx = { 0, 0 };
	ColorType _color;

	
	
};

