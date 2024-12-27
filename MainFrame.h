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
#include "Game.h"




class MainFrame : public wxFrame
{
	MainFrame* mainFramePointer;
	wxImagePanel* panel;
	wxStaticText* whoseTurnTextUnder;
	wxStaticText* whoWinsTextUnder;
	int PIXEL_SIZE_OF_PAWN;
	int PIXEL_SIZE_OF_CELL;
	int PIXEL_OFFSET_OF_BUTTON_POINT;
	int margin;
	float SCALE;
	wxString assetsFolder;
	wxSize buttonSize;

	Game* game;

public:
	MainFrame(const wxString& title, const long& style);
	void onPawnClick(wxCommandEvent& evt);
	void onPawnMoveClick(wxCommandEvent& evt);
	void createPawnMoveButton(PawnMove* move);
	void createPawnButton(Pawn* pawn, int pawnId);
	void clearPawnMoveButtons();
	wxPoint buttonPoint(int row, int col);
	void displayInfo(wxCommandEvent& evt);
	void surrenderGame(wxCommandEvent& evt);//?
	void startNewGame(wxCommandEvent& evt);
	void quitApp(wxCommandEvent& evt);
	void calculateButtonOffset();
	void generatePawnsOnBoard();
	void updateTexts();
};

