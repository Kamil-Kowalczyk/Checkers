#pragma once
#include <wx/wx.h>
#include <list>
#include <set>
#include "Pawn.h"


/*
* An enum for the type of the move
*/
enum MoveType {
	// some pawns are going to be captured during the move
	CAPTURE,

	// only movement is going to be made during the move
	MOVE,
};

/*
* A class representing a move for a pawn
*/
class PawnMove
{
public:
	// a row number where the pawn is going to be placed after the move completion
	int row;

	// a column number where the pawn is going to be placed after the move completion
	int col;

	// an unique number that identifies PawnMove. It is mandatory for creating a button that represents the move
	int id;

	// a pointer to a wxButton that after a click performs the move
	wxButton* moveButton;

	// a type of the move to be performed
	MoveType moveType;

	// a pointer to a Pawn that the move is meant for
	Pawn* pawn;

	// a set of Pawn pointers to be captured during the move
	std::set<Pawn*> pawnsToCapture;

	/*
	* Constructor
	* @param row - a row number where the pawn is going to be placed after the move completion
	* @param col - a column number where the pawn is going to be placed after the move completion
	* @param pawn - a pointer to a Pawn that the movement is meant for
	* @param moveType - a type of the move to be performed
	* @param id - an unique number that determines the order of every PawnMove
	*/
	PawnMove(int row, int col, Pawn* pawn, MoveType moveType, int id);
};

