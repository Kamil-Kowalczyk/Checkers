#include "MainFrame.h"

enum Direction {
	TOP_RIGHT,
	TOP_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

wxImagePanel* panel;
wxPanel* mainPanel;
wxButton* startMenuButtons[2];


Field board[8][8];

Pawn pawns[24];

MainFrame* mainFramePointer;

Pawn* pawnToMove;

Color whoseTurn = WHITE;

int size = 75;
int id = 0;

std::list<PawnMove*> pawnMoves;

bool isAnyBeatMove;

wxBoxSizer* sizer;

MainFrame::MainFrame(const wxString& title, const long& style) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style) {
	//wxPanel* panel = new wxPanel(this);
	mainFramePointer = this;
	/*mainPanel = new wxPanel(mainFramePointer, wxID_ANY, wxDefaultPosition);
	mainPanel->SetBackgroundColour(wxColor(255, 0, 0));*/
	//prepareStartMenu();
	//mainFramePointer->Bind(wxEVT_CHAR_HOOK, & MainFrame::showEscapeMenu, this);
	
	createBoard();

	putPawnsOnBoard();
	
	checkBoard();
}

void MainFrame::checkBoard() {
	int moveId = 0;
	int whiteBeated = 0;
	int blackBeated = 0;

	for (int i = 0; i < 24; i++) {
		Pawn* pawn = &pawns[i];

		//game ending
		if (!pawn->isOnBoard) {
			if (pawn->color == WHITE) {
				whiteBeated++;
			} else {
				blackBeated++;
			}
			continue;
		}
		if (pawn->color != whoseTurn) {
			continue;
		}

		int row = pawn->row;
		int col = pawn->col;

		

		if (pawn->color == WHITE && row != 0 && pawn->isOnBoard) {
			if (col != 0) {
				Field fieldtoMove = board[row - 1][col - 1];
				Field fieldToMoveForBeat = board[row - 2][col - 2];
				if (fieldtoMove.pawn == nullptr) {
					createPawnMove(row - 1, col - 1, pawn, moveId, MOVE);
				} else {
					if (fieldtoMove.pawn->color == !whoseTurn && fieldToMoveForBeat.pawn == nullptr) {
						createPawnMove(row - 2, col - 2, pawn, moveId, BEAT, row - 1, col - 1);
						isAnyBeatMove = true;
					}
				}

				// if (checkForBeatMove(row - 1, col - 1, row - 2, col - 2))
				// 	createPawnMove(row - 2, col - 2, pawn, moveId, BEAT, row - 1, col - 1);
				// else
				// 	createPawnMove(row - 1, col - 1, pawn, moveId, MOVE);
			}

			if (col != 7) {
				Field fieldtoMove = board[row - 1][col + 1];
				Field fieldToMoveForBeat = board[row - 2][col + 2];
				if (fieldtoMove.pawn == nullptr) {
					createPawnMove(row - 1, col + 1, pawn, moveId, MOVE);
				} else {
					if (fieldtoMove.pawn->color == !whoseTurn && fieldToMoveForBeat.pawn == nullptr) {
						createPawnMove(row - 2, col + 2, pawn, moveId, BEAT, row - 1, col + 1);
						isAnyBeatMove = true;
					}
				}
				// if (checkForBeatMove(row - 1, col + 1, row - 2, col + 2))
				// 	createPawnMove(row - 2, col + 2, pawn, moveId, BEAT, row - 1, col + 1);
				// else 
				// 	createPawnMove(row - 1, col + 1, pawn, moveId, MOVE);
			}
		}
		else if (pawn->color == BLACK  && row != 7 && pawn->isOnBoard) {
			if (col != 0) {
				Field fieldtoMove = board[row + 1][col - 1];
				Field fieldToMoveForBeat = board[row + 2][col - 2];
				if (fieldtoMove.pawn == nullptr) {
					createPawnMove(row + 1, col - 1, pawn, moveId, MOVE);
				} else {
					if (fieldtoMove.pawn->color == !whoseTurn && fieldToMoveForBeat.pawn == nullptr) {
						createPawnMove(row + 2, col - 2, pawn, moveId, BEAT, row + 1, col - 1);
						isAnyBeatMove = true;
					}
				}
				// if (checkForBeatMove(row + 1, col - 1, row + 2, col - 2))
				// 	createPawnMove(row + 2, col - 2, pawn, moveId, BEAT, row + 1, col - 1);
				// else
				// 	createPawnMove(row + 1, col - 1, pawn, moveId, MOVE);
			}
			if (col != 7) {
				Field fieldtoMove = board[row + 1][col + 1];
				Field fieldToMoveForBeat = board[row + 2][col + 2];
				if (fieldtoMove.pawn == nullptr) {
					createPawnMove(row + 1, col + 1, pawn, moveId, MOVE);
				} else {
					if (fieldtoMove.pawn->color == !whoseTurn && fieldToMoveForBeat.pawn == nullptr) {
						createPawnMove(row + 2, col + 2, pawn, moveId, BEAT, row - 1, col + 1);
						isAnyBeatMove = true;
					}
				}
				// if (checkForBeatMove(row + 1, col + 1, row + 2, col + 2))
				// 	createPawnMove(row + 2, col + 2, pawn, moveId, BEAT, row + 1, col + 1);
				// else
				// 	createPawnMove(row + 1, col + 1, pawn, moveId, MOVE);
			}
		}
	}

	if (blackBeated == 12) {
		wxLogMessage("White won the game");
	} 
	if (whiteBeated == 12) {
		wxLogMessage("Black won the game");
	}


}

void MainFrame::onPawnClick(wxCommandEvent& evt) {
	clearPawnMoveButtons();
	
	int pawnButtonId = evt.GetId();
	Pawn* pawn = &pawns[pawnButtonId - 1000];
	pawnToMove = nullptr;

	if (pawn->isMovable) {
		pawnToMove = pawn;
		MoveType wantedType;

		if (isAnyBeatMove)
			wantedType = BEAT;
		else
			wantedType = MOVE;

		for (int i = 0; i < pawnMoves.size(); i++) {
			auto iter = std::next(pawnMoves.begin(), i);
			PawnMove* pawnMove = *iter;

			if (pawnMove->pawns.back() == pawn && pawnMove->moveType == wantedType) {
				createPawnMoveButton(pawnMove);
			}
		}
	}
	

	/*if (pawns[evt.GetId() - 1000].color == whoseTurn) {
		pawnToMove = &pawns[evt.GetId() - 1000];
		Color pawnColor = pawnToMove->color;
		int x = pawnToMove->pawnButton->GetPosition().x;
		int y = pawnToMove->pawnButton->GetPosition().y;
		wxPoint coordinates[2];

		if (possibleMoveFields[0].moveButton != nullptr) {
			possibleMoveFields[0].moveButton->Destroy();
			possibleMoveFields[0].moveButton = nullptr;
		}

		if (possibleMoveFields[1].moveButton != nullptr) {
			possibleMoveFields[1].moveButton->Destroy();
			possibleMoveFields[1].moveButton = nullptr;

		}*/

		//todo if with red button

		/*if (pawnColor == BLACK) {
			coordinates[0] = wxPoint(x + 75, y + 75);
			coordinates[1] = wxPoint(x - 75, y + 75);

			possibleMoveFields[0].moveType = MOVE;
			possibleMoveFields[1].moveType = MOVE;

			for (int i = 0; i < 24; i++) {
				if (pawns[i].color != pawnToMove->color && pawns[i].onBoard == true) {
					if (pawns[i].pawnButton->GetPosition() == coordinates[0] && pawns[i].pawnButton->GetPosition() != wxPoint(coordinates[0].x + 75, coordinates[0].y + 75)) {
						coordinates[0] = wxPoint(coordinates[0].x + 75, coordinates[0].y + 75);
						possibleMoveFields[0].moveType = BEAT;
						pawnsToBeat[0] = &pawns[i];
					}
					if (pawns[i].pawnButton->GetPosition() == coordinates[1] && pawns[i].pawnButton->GetPosition() != wxPoint(coordinates[1].x - 75, coordinates[0].y + 75)) {
						coordinates[1] = wxPoint(coordinates[1].x - 75, coordinates[1].y + 75);
						possibleMoveFields[1].moveType = BEAT;
						pawnsToBeat[1] = &pawns[i];
					}
				}
			}
		}
		else {
			coordinates[0] = wxPoint(x + 75, y - 75);
			coordinates[1] = wxPoint(x - 75, y - 75);
			possibleMoveFields[0].moveType = MOVE;
			possibleMoveFields[1].moveType = MOVE;

			for (int i = 0; i < 24; i++) {
				if (pawns[i].color != pawnToMove->color && pawns[i].onBoard == true) {
					if (pawns[i].pawnButton->GetPosition() == coordinates[0] && pawns[i].pawnButton->GetPosition() != wxPoint(coordinates[0].x + 75, coordinates[0].y - 75)) {
						coordinates[0] = wxPoint(coordinates[0].x + 75, coordinates[0].y - 75);
						possibleMoveFields[0].moveType = BEAT;
						pawnsToBeat[0] = &pawns[i];
					}
					if (pawns[i].pawnButton->GetPosition() == coordinates[1] && pawns[i].pawnButton->GetPosition() != wxPoint(coordinates[1].x - 75, coordinates[0].y - 75)) {
						coordinates[1] = wxPoint(coordinates[1].x - 75, coordinates[1].y - 75);
						possibleMoveFields[1].moveType = BEAT;
						pawnsToBeat[1] = &pawns[i];
					}
				}
			}
		}
		
		if (possibleMoveFields[0].moveType == BEAT) {
			possibleMoveFields[0].moveButton = new wxButton(panel, 2000, "", coordinates[0], wxSize(45, 45), wxBORDER_NONE);
			possibleMoveFields[0].moveButton->SetBackgroundColour(wxColor(255, 0, 0));
			possibleMoveFields[0].moveButton->Bind(wxEVT_BUTTON, &MainFrame::onPossibleBeatFieldClick, mainFramePointer);
		}
		if (possibleMoveFields[1].moveType == BEAT) {
			possibleMoveFields[1].moveButton = new wxButton(panel, 2001, "", coordinates[1], wxSize(45, 45), wxBORDER_NONE);
			possibleMoveFields[1].moveButton->SetBackgroundColour(wxColor(255, 0, 0));
			possibleMoveFields[1].moveButton->Bind(wxEVT_BUTTON, &MainFrame::onPossibleBeatFieldClick, mainFramePointer);
		}

		if (possibleMoveFields[0].moveType == MOVE && possibleMoveFields[1].moveType == MOVE) {

			possibleMoveFields[0].moveButton = new wxButton(panel, 2000, "", coordinates[0], wxSize(45, 45), wxBORDER_NONE);
			possibleMoveFields[0].moveButton->SetBackgroundColour(wxColor(0, 255, 0));
			possibleMoveFields[0].moveButton->Bind(wxEVT_BUTTON, &MainFrame::onPossibleMoveFieldClick, mainFramePointer);

			possibleMoveFields[1].moveButton = new wxButton(panel, 2001, "", coordinates[1], wxSize(45, 45), wxBORDER_NONE);
			possibleMoveFields[1].moveButton->SetBackgroundColour(wxColor(0, 255, 0));
			possibleMoveFields[1].moveButton->Bind(wxEVT_BUTTON, &MainFrame::onPossibleMoveFieldClick, mainFramePointer);

		}
		arePossibleMoveFieldsShown = true;
	}*/
}

void MainFrame::onPawnMoveClick(wxCommandEvent& evt) {
	//wxLogMessage(wxString::Format("%d", pawnToMove->pawnButton->GetId()));

	auto iter = std::next(pawnMoves.begin(), evt.GetId());
	PawnMove* move = *iter;

	if (move->moveType == MOVE) {
		board[pawnToMove->row][pawnToMove->col].erasePawn();
		board[move->row][move->col].putPawn(pawnToMove);
		pawnToMove->pawnButton->SetPosition(wxPoint(move->col * size + (size * 0.2), move->row * size + (size * 0.2)));
	}
	else {
		board[pawnToMove->row][pawnToMove->col].erasePawn();
		beatPawn(board[move->rowToBeat][move->colToBeat].pawn);
		board[move->rowToBeat][move->colToBeat].erasePawn();
		board[move->row][move->col].putPawn(pawnToMove);
		pawnToMove->pawnButton->SetPosition(wxPoint(move->col * size + (size * 0.2), move->row * size + (size * 0.2)));
	}
	
	clearPawnMoveButtons();

	isAnyBeatMove = false;
	pawnMoves.clear();
	whoseTurn = whoseTurn == WHITE ? BLACK : WHITE;

	checkBoard();

	/*wxPoint possitionToMove = possibleMoveFields[evt.GetId() - 2000].moveButton->GetPosition();
	pawnToMove->pawnButton->SetPosition(possitionToMove);

	possibleMoveFields[0].moveButton->Destroy();
	possibleMoveFields[0].moveButton = nullptr;
	possibleMoveFields[0].moveType = NONE;

	possibleMoveFields[1].moveButton->Destroy();
	possibleMoveFields[1].moveButton = nullptr;
	possibleMoveFields[1].moveType = NONE;

	arePossibleMoveFieldsShown = false;
	whoseTurn = whoseTurn == WHITE ? BLACK : WHITE;*/
}

void MainFrame::onPossibleBeatFieldClick(wxCommandEvent& evt) {
	/*wxPoint possitionToMove = possibleMoveFields[evt.GetId() - 2000].moveButton->GetPosition();
	pawnToMove->pawnButton->SetPosition(possitionToMove);

	for (int i = 0; i < 2; i++) {
		if (i == evt.GetId() - 2000) {
			pawnsToBeat[i]->onBoard = false;
			pawnsToBeat[i]->pawnButton->Destroy();
			pawnsToBeat[i]->pawnButton = nullptr;
		}
	}

	if (possibleMoveFields[0].moveType == BEAT) {
		possibleMoveFields[0].moveButton->Destroy();
		possibleMoveFields[0].moveButton = nullptr;
		
	}

	if (possibleMoveFields[1].moveType == BEAT) {
		possibleMoveFields[1].moveButton->Destroy();
		possibleMoveFields[1].moveButton = nullptr;
		
	}

	possibleMoveFields[0].moveType = NONE;
	possibleMoveFields[1].moveType = NONE;

	arePossibleMoveFieldsShown = false;
	whoseTurn = whoseTurn == WHITE ? BLACK : WHITE;*/
}




void MainFrame::createPawnMoveButton(PawnMove* move) {
	int red = 0;
	int green = 0;

	if (move->moveType == MOVE)
		green = 255;
	else
		red = 255;

	move->moveButton = new wxButton(panel, move->id, "", wxPoint(move->col * size + (size * 0.2), move->row * size + (size * 0.2)),
		wxSize(0.6 * size, 0.6 * size), wxBORDER_NONE);
	move->moveButton->SetBackgroundColour(wxColor(red, green, 0));
	move->moveButton->Bind(wxEVT_BUTTON, &MainFrame::onPawnMoveClick, mainFramePointer);
}

void MainFrame::createPawnButton(Pawn* pawn) {
	wxString imageSrc;

	if (pawn->color == BLACK)
		imageSrc = "assets/black_pawn.png";
	else
		imageSrc = "assets/white_pawn.png";

	pawn->pawnButton = new wxBitmapButton(panel, 1000 + id, wxBitmap(wxImage(imageSrc, wxBITMAP_TYPE_PNG)),
		wxPoint(pawn->col * size + (size * 0.2), pawn->row * size + (size * 0.2)), wxSize(0.6 * size, 0.6 * size), wxBORDER_NONE);
	pawn->pawnButton->SetBackgroundColour(wxColor(107, 53, 17));
	id++;
}

void MainFrame::clearPawnMoveButtons() {
	PawnMove* pawnMove;
	int size = pawnMoves.size();
	for (int i = 0; i < size; i++) {
		auto iter = std::next(pawnMoves.begin(), i);
		pawnMove = *iter;
		if (pawnMove->moveButton != nullptr){
			pawnMove->moveButton->Destroy();
			pawnMove->moveButton = nullptr;
		}
	}
}

void MainFrame::createPawnMove(int row, int col, Pawn* pawn, int& moveId, MoveType moveType, int rowToBeat, int colToBeat) {
	PawnMove* pawnMove = new PawnMove(row, col, size, moveType, moveId, rowToBeat, colToBeat);
	pawnMove->pawns.push_back(pawn);
	pawnMove->moveButton = nullptr;
	pawn->isMovable = true;
	pawnMoves.push_back(pawnMove);
	//change the place of incrementation of moveId variable
	moveId++;
}

void MainFrame::createBoard() {
	wxInitAllImageHandlers();
	panel = new wxImagePanel(this, "assets/board.png", wxBITMAP_TYPE_PNG);
	bool whiteField = true;
	Color color;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (whiteField)
				color = WHITE;
			else
				color = BLACK;

			board[i][j] = Field(i, j, size, color);

			whiteField = !whiteField;
		}
		whiteField = !whiteField;
	}
}

void MainFrame::putPawnsOnBoard() {
	int index = 0;
	Color color = BLACK;

	for (int o = 0; o <= 1; o++) {
		for (int i = o * 5; i < o * 5 + 3; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j].color != BLACK)
					continue;

				pawns[index] = Pawn(color, i, j, size, false);
				pawns[index].isOnBoard = true;
				createPawnButton(&pawns[index]);
				board[i][j].putPawn(&pawns[index]);
				pawns[index].pawnButton->Bind(wxEVT_BUTTON, &MainFrame::onPawnClick, this);
				index++;
			}
		}
		color = WHITE;
	}
}

bool MainFrame::checkForBeatMove(int row, int col, int destRow, int destCol) {
	if (board[row][col].pawn != nullptr && board[row][col].pawn->color != whoseTurn && board[destRow][destCol].pawn == nullptr 
		&& destRow < 8 && destRow > 0 && destCol < 8 && destCol > 0) {
		isAnyBeatMove = true;
		return true;
	}
	return false;
}

void MainFrame::beatPawn(Pawn* pawn) {
	pawn->isOnBoard = false;
	pawn->pawnButton->Destroy();
	pawn->pawnButton = nullptr;
}

// void MainFrame::showEscapeMenu(wxKeyEvent& evt) {
// 	wxMessageBox("you have pressed esc key");
// 	evt.Skip();
// }

// void MainFrame::prepareStartMenu() {
// 	int screenWidth = wxSystemSettings::GetMetric( wxSYS_SCREEN_X);
// 	int screenHeight = wxSystemSettings::GetMetric( wxSYS_SCREEN_Y);
// 	startMenu = new StartMenu(mainFramePointer, screenWidth, screenHeight);
	
	
// }