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
	int size;
	bool isOnBoard;
	bool isMovable;
	//std::list<PawnMove*> possibleMoves;
	wxBitmapButton* pawnButton;
	//wxImagePanel* context;

	Pawn();
	Pawn(Color color, int row, int col, int size, bool isOnBoard);
	~Pawn();
	//void createButton();
};

