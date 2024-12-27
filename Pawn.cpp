#include "Pawn.h"

Pawn::Pawn(Color color, int row, int col, bool onBoard) {
	this->color = color;
	this->col = col;
	this->row = row;
	this->isOnBoard = onBoard;
	this->isMovable = false;
	this->isQueen = false;
	this->becameQueenInLastMove = false;
	this->pawnButton = nullptr;
}
