#pragma once

#include <wx/wx.h>


/*
* An enum representing a color
*/
enum Color {
	// white color
	WHITE, 

	// black color
	BLACK
};

/*
* A class representing a single pawn on the game board
* @field color -
* @field col -
* @field row -
* @field isOnBoard -
* @field isMovable -
* @field isQueen -
* @field pawnButton -
*/
class Pawn
{
public:
	// a color of the pawn
	Color color;

	// a column number where the pawn is standing
	int col;

	// a row number where the pawn is standing
	int row;

	// determines if the pawn is on the board
	bool isOnBoard;

	// determines if the pawn has any moves to perform
	bool isMovable;

	// determines if the pawn is a queen pawn
	bool isQueen;

	// a pointer to a wxBitmapButton representing the pawn on the board
	wxBitmapButton* pawnButton;

	/*
	* Constructor
	* @param color - a color of the pawn
	* @param row - a row number where the pawn is standing
	* @param col - a column number where the pawn is standing
	* @param isOnBoard - determines if the pawn is on the board
	*/
	Pawn(Color color, int row, int col, bool isOnBoard);
};

