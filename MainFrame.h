#pragma once

#include <wx/wx.h>
#include "Pawn.h"
#include "Field.h"
#include "ImagePanel.h"
#include "PawnMove.h"
#include <list>
#include <set>
#include <wx/display.h>
#include "Game.h"

/*
* A class holding the entire app GUI
*/
class MainFrame : public wxFrame
{
	// a pointer to the MainFrame object itself
	MainFrame* mainFramePointer;

	// a pointer to wxImagePanel which is the game board
	wxImagePanel* panel;
	
	// a pointer to wxStaticText where is displayed whose turn it is
	wxStaticText* whoseTurnTextUnder;

	// a pointer to wxStaticText where is displayed who is winning
	wxStaticText* whoWinsTextUnder;

	// the size of a single pawn in pixels, used as both its width and height
	int PIXEL_SIZE_OF_PAWN;

	// the size a single field on the game board in pixels, used as both its width and height
	int PIXEL_SIZE_OF_CELL;

	// the offset of the wxPoint
	int PIXEL_OFFSET_OF_BUTTON_POINT;

	// the margin used for ui elements on the left side of the app
	int margin;

	// the scale of the app
	float SCALE;

	// the name of the folder with the assets
	wxString assetsFolder;

	// the size of the Pawn
	wxSize buttonSize;

	// a pointer to the Game class object
	Game* game;

public:
	/*
	* Constructor, checks the available space on the screen and sets proper dimensions and a proper assets folder.
	* Creates the ui: the game board, a panel on the left side of the app where are displayed some information and buttons.
	* Creates a new Game object.
	* Calls generatePawnsOnBoard method and updateTexts method
	* @param title - title of the frame
	* @param style - style of the frame
	*/
	MainFrame(const wxString& title, const long& style);

	/*
	* Handles a click event performed on a button representing a pawn
	* @param evt - the click event sent by the pawn button
	*/
	void onPawnClick(wxCommandEvent& evt);

	/*
	* Handles a click event performed on a button representing a pawn move
	* @param - the click event sent by the pawn move button
	*/
	void onPawnMoveClick(wxCommandEvent& evt);

	/*
	* Creates a wxButton representing a pawn move
	* @param move - pointer to the PawnMove object
	*/
	void createPawnMoveButton(PawnMove* move);

	/*
	* Creates a wxBitmapButton representing a pawn on the board.
	* @param pawn - pointer to the Pawn object
	* @param pawnId - an id of the pawn
	*/
	void createPawnButton(Pawn* pawn, int pawnId);

	/*
	* Destroys wxButton for every PawnMove
	*/
	void clearPawnMoveButtons();

	/*
	* Calculates a point where the button should be placed based on the passed row and col number
	* @param row - a row number of the game board, where the button should be placed
	* @param col - a column number of the game board, where the button should be placed
	* @return a wxPoint
	*/
	wxPoint buttonPoint(int row, int col);

	/*
	* Displays some information about the author
	* @param evt - the click event sent by the button
	*/
	void displayInfo(wxCommandEvent& evt);

	/*
	* Displays a message which asks the user if they want to start a new game, if so a new game will be started otherwise the current game will be continiued
	* @param evt - the click event sent by the button
	*/
	void startNewGame(wxCommandEvent& evt);

	/*
	* Displays a message that asks the user if they want to quit the app, if so the app will be closed otherwise the app will still be running
	* @param evt - the click event sent by the button
	*/
	void quitApp(wxCommandEvent& evt);

	/*
	* Calculates the offset for the wxPoint to let the button on the game board to be in the center of the field
	*/
	void calculateButtonOffset();

	/*
	* Creates pawns on the game board by calling createPawnButton and binds to them onPawnClick as the click handler
	*/
	void generatePawnsOnBoard();

	/*
	* Update texts visible in the left panel based on e.g. whose turn is now, how much lead the player has etc
	*/
	void updateTexts();
};

