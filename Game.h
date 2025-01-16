#pragma once

#include "Field.h"
#include "Pawn.h"
#include "list"
#include "PawnMove.h"
#include "direction/Direction.h"

/*
* A class holding the game's logic
*/
class Game {
public:
	// an array of the board fields
	Field* board[8][8];

	// an array of all pawns from the game
	Pawn* pawns[24];

	// a pointer to the pawn which will be moved
	Pawn* pawnToMove;

	// determines if it is white's turn or black's
	Color whoseTurn;

	// a list of all moves that pawns can make during the turn
	std::list<PawnMove*> pawnMoves;

	// determines if any capturing move exists
	bool isAnyCapturingMove;

	// number of maximum pawns that can be captured during one move
	int maxPawnsToCapture;

	// number of white pawns that are already captured
	int whiteCaptured = 0;

	// number of black pawns that are already captured
	int blackCaptured = 0;

	/*
	* Constructor, initilizes the game board and sets default values for the class properties
	*/
	Game();

	/*
	* Sums captured pawns and calls checkForMove method in order to check the board for moves
	*/
	void checkBoard();

	/*
	* Checks for available capturing moves and simple moves.
	* @param startRow - a row number where the search of moves should begin
	* @param startCol - a column number where the search of moves should begin
	* @param pawn - a pointer to a Pawn object for which moves are being searched
	* @param directionToErase - a Direction where the function should not search for moves
	* @param pawnsToCapture - a set of pointers to the Pawn objects which can be captured during the move (in initial state the set is empty)
	* @param queenDirection - a Direction from which the queen has come in the previous move (NONE means that it is her start position)
	*/
	void checkForMove(int startRow, int startCol, Pawn* pawn, Direction directionToErase, std::set<Pawn*> pawnsToCapture, Direction queenDirection = NONE);
	
	/*
	* Creates a PawnMove and pushes it to the pawnMoves (a property of the Game class)
	* @param row - a row number where the pawn is going to be placed after the move completion
	* @param col - a column number where the pawn is going to be placed after the move completion
	* @param pawn - a pointer to a Pawn object
	* @param moveId - an unique number to identify the move (usefull for buttons representing the move)
	* @param moveType - determines if the move is a MOVE or a CAPTURE
	* @param pawnsToCapture - pawns that are going to be beated during the move
	*/
	void createPawnMove(int row, int col, Pawn* pawn, int moveId, MoveType moveType, std::set<Pawn*> pawnsToCapture = {});
	
	/*
	* Puts pawns on the Fields of the board by caling putPawn(a method  of the Game class)
	*/
	void putPawnsOnBoard();

	/*
	* Removes a pawn from the board and destroys its wxBitmapButton
	* @param pawn - a pointer to a pawn to be captured
	*/
	void capturePawn(Pawn* pawn);

	/*
	* Starts a new game. Sets default values for properties of the Game class
	*/
	void newGame();

	/*
	* Performs the move with the passed id for pawnToMove. 
	* @param moveId - an id of the move to be executed
	* @return a boolean which says if the pawn has become a queen during the po move
	*/
	bool performMove(int moveId);

	/*
	* Sets pawnToMove property with a pointer to a pawn that has triggered the click event if the pawn is movable.
	* Then searches for moves that the selected pawn can make
	* @param pawnId - an id of the pawn that has been clicked
	* @return moves that the pawn can perrform
	*/
	std::list<PawnMove*> handlePawnClick(int pawnId);

	/*
	* Returns a pawn corresponding to the passed id 
	* @param pawnId - an id of a pawn
	* @return a pointer to the Pawn object
	*/
	Pawn* getPawnById(int pawnId);
};