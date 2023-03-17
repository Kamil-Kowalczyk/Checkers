#pragma once

#include "Pawn.h"

class Field
{
public:
	int row, col, size;
	Pawn *pawn;
	Color color;

	Field();
	Field(int& row, int& col, Color color);
	~Field();

	void putPawn(Pawn* pawn);
	void erasePawn();
};

