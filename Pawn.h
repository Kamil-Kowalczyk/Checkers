#pragma once

#include <wx/wx.h>
//#include "PawnMove.h"
//#include <list>

enum Color {
	WHITE,
	BLACK
};
class Pawn
{
public:
	Color color;
	int col;
	int row;
	bool isOnBoard;
	bool isMovable;
	bool isQueen;
	//std::list<PawnMove*> possibleMoves;
	wxBitmapButton* pawnButton;
	//wxImagePanel* context;

	Pawn();
	Pawn(Color color, int row, int col, bool isOnBoard);
	~Pawn();
	//void createButton();
};

