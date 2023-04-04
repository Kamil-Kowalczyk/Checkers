#pragma once

#include <wx/wx.h>
#include "Pawn.h"
#include "Field.h"
#include "ImagePanel.h"
#include "PawnMove.h"
#include <list>
#include <set>
#include <wx/display.h>
#include <math.h>

enum Direction {
	TOP_RIGHT,
	TOP_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
	NONE
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const long& style);
	void onPawnClick(wxCommandEvent& evt);
	void onPawnMoveClick(wxCommandEvent& evt);
	void checkBoard();
	void createPawnMoveButton(PawnMove* move);
	void createPawnButton(Pawn* pawn);
	void clearPawnMoveButtons();
	void createPawnMove(int row, int col, Pawn* pawn, int moveId, MoveType moveType, std::set<Pawn*> pawnsToBeat = {});
	void createBoard();
	void putPawnsOnBoard();
	void checkForPawnBeatMove(Pawn* pawn);
	void checkForMove(int row, int col, Pawn* pawn, Direction directionToErase, std::set<Pawn*> pawnsToBeat, Direction queenDirection = NONE);
	void beatPawn(Pawn* pawn);
	void incrementDestinationCoordinates(Direction direction, int& row, int& col);
	void checkForQueenMove();
	wxPoint buttonPoint(int row, int col);
	Direction getCounterDirection(Direction direction);
	void displayInfo(wxCommandEvent& evt);
	void surrenderGame(wxCommandEvent& evt);
	void startNewGame(wxCommandEvent& evt);
	void newGame();
	void quitApp(wxCommandEvent& evt);
};

