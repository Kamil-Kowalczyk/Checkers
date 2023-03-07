#pragma once
#include <wx/wx.h>
#include <list>
#include <set>
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
	Pawn* pawn;
	std::set<Pawn*> pawnsToBeat;
	//wxImagePanel* context;
	//MainFrame* mainFramePointer;
	PawnMove(int row, int col, Pawn* pawn, int size, MoveType moveType, int id);
	~PawnMove();
	//void createButton();
};

