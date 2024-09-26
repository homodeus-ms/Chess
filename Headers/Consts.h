#pragma once

const int32 G_WINSIZE_X = 1280;
const int32 G_WINSIZE_Y = 720;

const int32 G_BOARD_LENGTH = 640;

const int32 G_BOARD_POSX = G_WINSIZE_X / 2;
const int32 G_BOARD_POSY = G_WINSIZE_Y / 2;
const int32 G_BOARD_STARTX = G_BOARD_POSX - G_BOARD_LENGTH / 2;
const int32 G_BOARD_STARTY = G_BOARD_POSY - G_BOARD_LENGTH / 2;

const int32 G_TILE_SIZE = 80;
const int32 G_TILE_COUNT = 8;
const int32 G_PIECE_SIZE = 64;

const VectorInt G_AI_TRASH_TRAY_POS = { 160, 190 };
const VectorInt G_PLAYER_TRASH_TRAY_POS = { 160, 530 };

const VectorInt G_MOVE_BACK_BOX_LEFT_TOP = { 995, 620 };
const VectorInt G_MOVE_BACK_BOX_RIGHT_BOTTOM = { 1250, 680 };

const VectorInt G_CAN_GO_TOGGLE_BOX_LEFT_TOP = { 995, 40 };
const VectorInt G_CAN_GO_TOGGLE_BOX_RIGHT_BOTTOM= { 1250, 100 };

const VectorInt G_SHOW_ATTACK_ZONE_LEFT_TOP = { 995, 120 };
const VectorInt G_SHOW_ATTACK_ZONE_RIGHT_BOTTOM = { 1250, 180 };

const VectorInt BONUS_AREA_LEFT_TOP = { 2, 2 };
const VectorInt BONUS_AREA_RIGHT_BOTTOM = { 5, 5 };

// K 10000 Q 500 R 300 B 200 N 200 P50 
const int32 PIECE_VALUE[6] = { 10000, 500, 300, 200, 200, 50 };