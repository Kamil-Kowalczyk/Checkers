#pragma once

#include <wx/wx.h>
#include "Pawn.h"
#include "Field.h"
#include "ImagePanel.h"
#include "PawnMove.h"
#include <list>
#include <set>
#include <wx/display.h>

enum Direction {
	TOP_RIGHT,
	TOP_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const long& style);
	void onPawnClick(wxCommandEvent& evt);
	void onPawnMoveClick(wxCommandEvent& evt);
	void onPossibleBeatFieldClick(wxCommandEvent& evt);
	void checkBoard();
	void createPawnMoveButton(PawnMove* move);
	void createPawnButton(Pawn* pawn);
	void clearPawnMoveButtons();
	void createPawnMove(int row, int col, Pawn* pawn, int& moveId, MoveType moveType, int rowToBeat = 100, int colToBeat = 100);
	void createBoard();
	void putPawnsOnBoard();
	void checkForPawnBeatMove(Pawn* pawn);
	bool checkForBeatMove(int row, int col, int destRow, int destCol);
	void beatPawn(Pawn* pawn);
	void getDestinationCoordinates(Direction direction, int& row, int& col);
};

