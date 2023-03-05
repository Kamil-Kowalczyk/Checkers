#pragma once
#include <wx/wx.h>
#include <list>
#include "Pawn.h"


enum MoveType {
	BEAT,
	MOVE,
};

class PawnMove
{
public:
	int row;
	int col;
	int size;
	int id;
	wxButton* moveButton;
	MoveType moveType;
	std::list<Pawn*> pawns;
	int rowToBeat;
	int colToBeat;
	//wxImagePanel* context;
	//MainFrame* mainFramePointer;
	PawnMove(int row, int col, int size, MoveType moveType, int id, int rowToBeat, int colToBeat);
	~PawnMove();
	//void createButton();
};

