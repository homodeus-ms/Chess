#pragma once

enum LayerType
{
	LAYER_BACKGROUND,
	LAYER_OBJECT,
	LAYER_UI,
	LAYER_MAX_COUNT,
};


enum GameLevel : uint8
{
	LEVEL_EASY,
	LEVEL_NORMAL,
	LEVEL_HARD,
};

//enum class PieceType
//{
//	Pawn,
//	Knight,
//	Bishop,
//	Rook,
//	Queen,
//	King,
//	PieceCount,
//};

enum PieceType
{
	TYPE_KING,
	TYPE_QUEEN,
	TYPE_ROOK,
	TYPE_BISHOP,
	TYPE_KNIGHT,
	TYPE_PAWN,
	
	PIECE_COUNT
};

enum PieceChar : uint8
{
	BK = 0,
	BQ = 1,
	BR = 2,
	BB = 3,
	BN = 4,
	BP = 5,
	WK = 6,
	WQ = 7,
	WR = 8,
	WB = 9,
	WN = 10,
	WP = 11,
	NP = 12,
};

enum class ColorType
{
	Black,
	White,
};

enum class PlayerType
{
	Human,
	Ai,
};

enum Turn
{
	PLAYER_TURN,
	AI_TURN,
};

enum Zone : uint8
{
	Z_None,
	Z_Red,    // Attackable Cells
	Z_Orange, // Attack root to King
	Z_Yellow,  // can't move pos (to preserve king)
};