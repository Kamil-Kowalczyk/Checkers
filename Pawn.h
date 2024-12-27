#pragma once

#include <wx/wx.h>


/*
* An enum representing a color
*/
enum Color {
	WHITE,
	BLACK
};

/*
* A class representing a single pawn on the game 
* @field color - a color of the pawn
* @field col - a column number where the pawn is standing
* @field row - a row number where the pawn is standing
* @field isOnBoard - determines if the pawn is on the board
* @field isMovable - determines if the pawn has any moves to perform
* @field isQueen - determines if the pawn is a queen pawn
* @field becameQueenInLastMove - determines if the pawn has became a queen in the last move
* @field pawnButton - pointer to a wxBitmapButton representing the pawn on the board
*/
class Pawn
{
public:
	Color color;
	int col;
	int row;
	bool isOnBoard;
	bool isMovable;
	bool isQueen;
	bool becameQueenInLastMove;
	wxBitmapButton* pawnButton;

	/*
	* Creates an object of the Pawn class
	* @param color - a color of the pawn
	* @param row - a row number where the pawn is standing
	* @param col - a column number where the pawn is standing
	* @param isOnBoard - determines if the pawn is on the board
	*/
	Pawn(Color color, int row, int col, bool isOnBoard);
};

