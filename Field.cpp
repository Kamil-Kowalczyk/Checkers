#include "Field.h"

Field::Field(int row, int col, Color color) {
	this->col = col;
	this->row = row;
	this->color = color;
	this->pawn = nullptr;
}

void Field::putPawn(Pawn* pawn) {
	this->pawn = pawn;
	this->pawn->col = this->col;
	this->pawn->row = this->row;
}
void Field::erasePawn() {
	this->pawn = nullptr;
}