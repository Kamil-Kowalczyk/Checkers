#pragma once

#include "Pawn.h"

/*
* A class representing a single field on the game board
* @field row - the number of the row on the game board
* @field col - the number of the column on the game board
* @field pawn - a pointer to a pawn standing on the field
* @field color - a color of the field
*/

class Field
{
public:
	int row, col;
	Pawn *pawn;
	Color color;


	/*
	* Creates an object of the Field class
	* @param row - a row number
	* @param col - a column number
	* @param color - a color of the field
	*/
	Field(int row, int col, Color color);

	/*
	* Puts a pawn on the field and sets the pawn column and row number 
	* corresponding to the field row and column number
	* @param pawn - a pointer to a Pawn object
	*/
	void putPawn(Pawn* pawn);

	/*
	* Removes a pawn from the field
	*/
	void erasePawn();
};

