#pragma once
#include <wx/wx.h>
#include <list>
#include <set>
#include "Pawn.h"


/*
An enum for types of moves where:
- BEAT means that some pawns are going to be beaten during the move
- MOVE means that only a movement is going to be made during the move
*/
enum MoveType {
	BEAT,
	MOVE,
};

/*
A class representing a move for a pawn

@field row - a row number where the pawn is going to be placed after the move completion
@field col - a column number where the pawn is going to be placed after the move completion
@field id - ?????????
@field moveButton - a pointer to a wxButton that after a click performs the move
@field moveType - a type of the move to be performed
@field pawn - a pointer to a Pawn that the move is meant for
@field pawnsToBeat - a set of pointers to Pawns to be beated during the move
*/
class PawnMove
{
public:
	int row;
	int col;
	int id;
	wxButton* moveButton;
	MoveType moveType;
	Pawn* pawn;
	std::set<Pawn*> pawnsToBeat;
	/*
	Creates an object of the PawnMove class

	@param row - a row number where the pawn is going to be placed after the move completion
	@param col - a column number where the pawn is going to be placed after the move completion
	@param pawn - a pointer to a Pawn that the movement is meant for
	@param moveType - a type of the move to be performed
	@param id - ?????
	*/
	PawnMove(int row, int col, Pawn* pawn, MoveType moveType, int id);
};

