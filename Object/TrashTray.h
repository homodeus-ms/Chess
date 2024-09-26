#pragma once
#include "Actor.h"

class Piece;

class TrashTray : public Actor
{
	using Super = Actor;

public:
	TrashTray(Texture* texture, bool isPlayers);
	virtual ~TrashTray() {}

	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);

	void DeleteObjects();

	void AddTrash(Piece* piece, bool isPromotedPawn = false);
	void RemovePieceInTray(Piece* piece);
	Piece* PickPieceForPromotionOrNull();
	Piece* GetPromotionPieceForAi();
	const VectorInt GetTrayPos(uint32 index);

	bool IsPlayersTray() const { return _isPlayers; }
	bool IsEmpty() const { return _trashes.empty(); }

private:
	bool IsValidPos(const VectorInt& pos) const;
	Piece* GetClickedPieceOrNull();

private:
	VectorInt _leftTop;
	VectorInt _rightBottom;
	vector<Piece*> _trashes;
	bool _isPlayers = false;
	
	// Picking For Promotion
	Texture* _pickingTexture = nullptr;
	bool _isPicking = false;
	VectorInt _drawPosLeftTop = { -1, -1 };
};

