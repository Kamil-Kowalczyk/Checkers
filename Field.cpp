#include "Field.h"

Field::Field() {
	/*this->col = 0;
	this->row = 0;
	this->size = 0;
	this->color = NONE;
	this->pawn = nullptr;*/
}
Field::Field(int& row, int& col, int& size, Color color) {
	this->col = col;
	this->row = row;
	this->size = size;
	this->color = color;
	this->pawn = nullptr;
}
Field::~Field() {}

void Field::putPawn(Pawn* pawn) {
	this->pawn = pawn;
	this->pawn->col = this->col;
	this->pawn->row = this->row;
}
void Field::erasePawn() {
	this->pawn = nullptr;
}