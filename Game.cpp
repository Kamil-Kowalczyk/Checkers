#include "Game.h"

Game::Game() {
	pawnToMove = nullptr;
	isAnyCapturingMove = false;
	maxPawnsToCapture = 0;
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
	isAnyCapturingMove = false;
	maxPawnsToCapture = 0;
	whoseTurn = WHITE;
	pawnMoves = {};

	putPawnsOnBoard();
	checkBoard();
}

void Game::createPawnMove(int row, int col, Pawn* pawn, int moveId, MoveType moveType, std::set<Pawn*> pawnsToCapture) {
	PawnMove* pawnMove = new PawnMove(row, col, pawn, moveType, moveId);
	pawnMove->moveButton = nullptr;
	pawnMove->pawnsToCapture = pawnsToCapture;
	pawn->isMovable = true;
	pawnMoves.push_back(pawnMove);
}

void Game::checkBoard() {
	whiteCaptured = 0;
	blackCaptured = 0;
	maxPawnsToCapture = 0;

	for (int i = 0; i < 24; i++) {
		Pawn* pawn = pawns[i];
		if (!pawn->isOnBoard) {
			if (pawn->color == WHITE) {
				whiteCaptured += 1;
			}
			else {
				blackCaptured += 1;
			}
		}
		else if (pawn->color == whoseTurn) {
			std::set<Pawn*> pawnsToCapture = {};
			checkForMove(pawn->row, pawn->col, pawn, NONE, pawnsToCapture);
		}
	}
}

void Game::checkForMove(int startRow, int startCol, Pawn* pawn, Direction directionToErase, std::set<Pawn*> pawnsToCapture, Direction queenDirection) {
	std::set <Direction> directionsWithMovementOnly = {};
	std::set<Direction> directions = { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT };
	std::set<Pawn*> pawnsToCaptureLocal;
	int numberOfPawnsToCaptureAtBeginning = pawnsToCapture.size();
	directions.erase(directionToErase);
	if (pawnsToCapture.size() == 0) {
		if (pawn->isQueen) {
			directionsWithMovementOnly = { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT };
		}
		else {
			if (pawn->color == WHITE) {
				directionsWithMovementOnly.insert(TOP_LEFT);
				directionsWithMovementOnly.insert(TOP_RIGHT);
			}
			else {
				directionsWithMovementOnly.insert(BOTTOM_LEFT);
				directionsWithMovementOnly.insert(BOTTOM_RIGHT);
			}
		}
		if (queenDirection != NONE) {
			directionsWithMovementOnly = { queenDirection };
			directions = { queenDirection };
		}
	}

	for (Direction direction : directions) {
		int destRow = startRow;
		int destCol = startCol;
		pawnsToCaptureLocal = pawnsToCapture;
		incrementDestinationCoordinates(direction, destRow, destCol);
		if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7) {
			Field* field = board[destRow][destCol];
			if (field->pawn != nullptr) {
				if (field->pawn->color != whoseTurn && pawnsToCapture.count(field->pawn) == 0) {
					Pawn* pawnToCapture = field->pawn;
					incrementDestinationCoordinates(direction, destRow, destCol);
					if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7) {
						field = board[destRow][destCol];
						if (field->pawn == nullptr) {
							pawnsToCaptureLocal.insert(pawnToCapture);
							checkForMove(destRow, destCol, pawn, getCounterDirection(direction), pawnsToCaptureLocal);
						}
					}
				}
			}
			else if (directionsWithMovementOnly.count(direction) == 1) {
				createPawnMove(destRow, destCol, pawn, pawnMoves.size(), MOVE);
				if (pawn->isQueen) {
					checkForMove(destRow, destCol, pawn, getCounterDirection(direction), pawnsToCaptureLocal, direction);
				}
			}
		}
	}

	if (numberOfPawnsToCaptureAtBeginning == pawnsToCapture.size() && pawnsToCapture.size() != 0) {
		createPawnMove(startRow, startCol, pawn, pawnMoves.size(), CAPTURE, pawnsToCapture);
		isAnyCapturingMove = true;
		if (pawnsToCapture.size() > maxPawnsToCapture) {
			maxPawnsToCapture = pawnsToCapture.size();
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



void Game::capturePawn(Pawn* pawn) {
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
				if (pawnMove->pawnsToCapture.size() == maxPawnsToCapture) {
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
		for (Pawn* pawn : move->pawnsToCapture) {
			capturePawn(board[pawn->row][pawn->col]->pawn);
			board[pawn->row][pawn->col]->erasePawn();
		}
		board[move->row][move->col]->putPawn(pawnToMove);
	}

	  isAnyCapturingMove = false;
	
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
	if (pawnId < 0 || pawnId > 23)
		return nullptr;
	return pawns[pawnId];
}