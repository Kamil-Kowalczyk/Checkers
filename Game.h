#pragma once

#include "Field.h"
#include "Pawn.h"
#include "list"
#include "PawnMove.h"
#include "direction/Direction.h"


class Game {
public:
	Field* board[8][8];
	Pawn* pawns[24];
	Pawn* pawnToMove;
	Color whoseTurn;
	std::list<PawnMove*> pawnMoves;
	bool isAnyBeatMove;
	int maxPawnsToBeat;
	int whiteBeated = 0;
	int blackBeated = 0;

	Game();
	void checkBoard();
	void checkForMove(int row, int col, Pawn* pawn, Direction directionToErase, std::set<Pawn*> pawnsToBeat, Direction queenDirection = NONE);
	void createPawnMove(int row, int col, Pawn* pawn, int moveId, MoveType moveType, std::set<Pawn*> pawnsToBeat = {});
	void putPawnsOnBoard();
	void beatPawn(Pawn* pawn);
	void newGame();
	bool performMove(int moveId);
	std::list<PawnMove*> handlePawnClick(int pawnId);
	Pawn* getPawnById(int pawnId);
};