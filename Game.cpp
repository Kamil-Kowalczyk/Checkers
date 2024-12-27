#include "Game.h"

Game::Game() {
	pawnToMove = nullptr;
	isAnyBeatMove = false;
	maxPawnsToBeat = 0;
	whoseTurn = WHITE;
	pawnMoves = {};

	bool whiteField = true;
	Color color;


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (whiteField)
				color = WHITE;
			else
				color = BLACK;

			board[i][j] = new Field(i, j, color);

			whiteField = !whiteField;
		}
		whiteField = !whiteField;
	}
	putPawnsOnBoard();
	checkBoard();	
}

void Game::newGame() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j]->erasePawn();
		}
	}

	for (Pawn* pawn : pawns) {
		if (pawn->pawnButton != nullptr) {
			pawn->pawnButton->Destroy();
			pawn->pawnButton = nullptr;
		}
		delete pawn;
		pawn = nullptr;
	}
	for (PawnMove* pawnMove : pawnMoves) {
		if (pawnMove->moveButton != nullptr) {
			pawnMove->moveButton->Destroy();
			pawnMove->moveButton = nullptr;
		}
	}

	pawnMoves.clear();

	pawnToMove = nullptr;
	isAnyBeatMove = false;
	maxPawnsToBeat = 0;
	whoseTurn = WHITE;
	pawnMoves = {};

	putPawnsOnBoard();
	checkBoard();
}

void Game::createPawnMove(int row, int col, Pawn* pawn, int moveId, MoveType moveType, std::set<Pawn*> pawnsToBeat) {
	PawnMove* pawnMove = new PawnMove(row, col, pawn, moveType, moveId);
	pawnMove->moveButton = nullptr;
	pawnMove->pawnsToBeat = pawnsToBeat;
	pawn->isMovable = true;
	pawnMoves.push_back(pawnMove);
}

void Game::checkBoard() {
	whiteBeated = 0;
	blackBeated = 0;
	maxPawnsToBeat = 0;

	for (int i = 0; i < 24; i++) {
		Pawn* pawn = pawns[i];
		if (!pawn->isOnBoard) {
			if (pawn->color == WHITE) {
				whiteBeated += 1;
			}
			else {
				blackBeated += 1;
			}
		}
		else if (pawn->color == whoseTurn) {
			std::set<Pawn*> pawnsToBeat = {};
			checkForMove(pawn->row, pawn->col, pawn, NONE, pawnsToBeat);
		}
	}
}

void Game::checkForMove(int row, int col, Pawn* pawn, Direction directionToErase, std::set<Pawn*> pawnsToBeat, Direction queenDirection) {
	std::set <Direction> directionsWithoutBeat = {};
	std::set<Direction> directions = { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT };
	std::set<Pawn*> pawnsToBeatLocal;
	directions.erase(directionToErase);
	if (pawnsToBeat.size() == 0) {
		if (pawn->isQueen) {
			directionsWithoutBeat = { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT };
		}
		else {
			if (pawn->color == WHITE) {
				directionsWithoutBeat.insert(TOP_LEFT);
				directionsWithoutBeat.insert(TOP_RIGHT);
			}
			else {
				directionsWithoutBeat.insert(BOTTOM_LEFT);
				directionsWithoutBeat.insert(BOTTOM_RIGHT);
			}
		}
		if (queenDirection != NONE) {
			directionsWithoutBeat = { queenDirection };
			directions = { queenDirection };
		}
	}
	int numberOfPawnsToBeatAtBeginning = pawnsToBeat.size();

	for (Direction direction : directions) {
		int destRow = row;
		int destCol = col;
		pawnsToBeatLocal = pawnsToBeat;
		incrementDestinationCoordinates(direction, destRow, destCol);
		if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7) {
			Field* field = board[destRow][destCol];
			if (field->pawn != nullptr) {
				if (field->pawn->color != whoseTurn && pawnsToBeat.count(field->pawn) == 0) {
					Pawn* pawnToBeat = field->pawn;
					incrementDestinationCoordinates(direction, destRow, destCol);
					if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7) {
						field = board[destRow][destCol];
						if (field->pawn == nullptr) {
							pawnsToBeatLocal.insert(pawnToBeat);
							checkForMove(destRow, destCol, pawn, getCounterDirection(direction), pawnsToBeatLocal);
						}
					}
				}
			}
			else if (directionsWithoutBeat.count(direction) == 1) {
				createPawnMove(destRow, destCol, pawn, pawnMoves.size(), MOVE);
				if (pawn->isQueen) {
					checkForMove(destRow, destCol, pawn, getCounterDirection(direction), pawnsToBeatLocal, direction);
				}
			}
		}
	}
	if (numberOfPawnsToBeatAtBeginning == pawnsToBeat.size() && pawnsToBeat.size() != 0) {
		createPawnMove(row, col, pawn, pawnMoves.size(), BEAT, pawnsToBeat);
		isAnyBeatMove = true;
		if (pawnsToBeat.size() > maxPawnsToBeat) {
			maxPawnsToBeat = pawnsToBeat.size();
		}
	}
}

void Game::putPawnsOnBoard() {
	int index = 0;
	Color color = BLACK;

	for (int o = 0; o <= 1; o++) {
		for (int i = o * 5; i < o * 5 + 3; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j]->color != BLACK)
					continue;

				pawns[index] = new Pawn(color, i, j, false);
				pawns[index]->isOnBoard = true;
				board[i][j]->putPawn(pawns[index]);
				index++;
			}
		}
		color = WHITE;
	}
}



void Game::beatPawn(Pawn* pawn) {
	pawn->isOnBoard = false;
	pawn->pawnButton->Destroy();
	pawn->pawnButton = nullptr;
}

std::list<PawnMove*> Game::handlePawnClick(int pawnId) {
	Pawn* pawn = getPawnById(pawnId);
	pawnToMove = nullptr;

	std::list<PawnMove*> pawnAvailableMoves = {};

	if (pawn->isMovable) {
		pawnToMove = pawn;

		for (int i = 0; i < pawnMoves.size(); i++) {
			auto iter = std::next(pawnMoves.begin(), i);
			PawnMove* pawnMove = *iter;

			if (pawnMove->pawn == pawn) {
				if (pawnMove->pawnsToBeat.size() == maxPawnsToBeat) {
					pawnAvailableMoves.push_back(pawnMove);
				}
			}
		}
	}
	return pawnAvailableMoves;
}

bool Game::performMove(int moveId) {
	auto iter = std::next(pawnMoves.begin(), moveId);
	PawnMove* move = *iter;

	if (move->moveType == MOVE) {
		board[pawnToMove->row][pawnToMove->col]->erasePawn();
		board[move->row][move->col]->putPawn(pawnToMove);
	}
	else {
		board[pawnToMove->row][pawnToMove->col]->erasePawn();
		for (Pawn* pawn : move->pawnsToBeat) {
			beatPawn(board[pawn->row][pawn->col]->pawn);
			board[pawn->row][pawn->col]->erasePawn();
		}
		board[move->row][move->col]->putPawn(pawnToMove);
	}

	  isAnyBeatMove = false;
	
	whoseTurn = whoseTurn == WHITE ? BLACK : WHITE;

	Pawn* pawn = pawnToMove;
	if (pawn->row == 0 && pawn->color == WHITE && pawn->isQueen == false) {
		pawn->isQueen = true;
		return true;
	}
	if (pawn->row == 7 && pawn->color == BLACK && pawn->isQueen == false) {
		pawn->isQueen = true;
		return true;
	}

	return false;
}

Pawn* Game::getPawnById(int pawnId) {
	return pawns[pawnId];
}