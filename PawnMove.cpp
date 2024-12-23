#include "PawnMove.h"

PawnMove::PawnMove(int row, int col, Pawn* pawn, MoveType moveType, int id) {
	this->row = row;
	this->col = col;
	this->moveType = moveType;
	this->id = id;
	this->pawn = pawn;
	this->moveButton = nullptr;
}