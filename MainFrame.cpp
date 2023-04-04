#include "MainFrame.h"

MainFrame* mainFramePointer;
wxImagePanel* panel;
wxStaticText* whoseTurnTextUnder;
wxStaticText* whoWinsTextUnder;
int SIZE;
int margin;
float SCALE;
wxString assetsFolder; 
wxSize buttonSize;

Field* board[8][8];
Pawn* pawns[24];
Pawn* pawnToMove;
Color whoseTurn;
int id;
std::list<PawnMove*> pawnMoves;
bool isAnyBeatMove;
int maxPawnsToBeat;

MainFrame::MainFrame(const wxString& title, const long& style) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style) {
	mainFramePointer = this;
	/*mainPanel = new wxPanel(mainFramePointer, wxID_ANY, wxDefaultPosition);
	mainPanel->SetBackgroundColour(wxColor(255, 0, 0));*/

	wxDisplay* display = new wxDisplay(wxDisplay::GetFromWindow(mainFramePointer));
	wxRect* screen = new wxRect(display->GetClientArea());
	if (screen->height > 1600) {
		SCALE = 2;
		SIZE = 200;
		assetsFolder = "x2.0";
	} else if (screen->height > 1000) {
		SCALE = 1;
		SIZE = 100;
		assetsFolder = "x1.0";
	} else {
		SCALE = 0.5;
		SIZE = 66;
		assetsFolder = "x0.5";
	}
	//wxLogMessage(wxString::Format("%d", display->GetScaleFactor()));
	delete display, screen;
	buttonSize = wxSize(SIZE, SIZE);
	margin = SCALE * 20;

	auto font = this->GetFont();
	font.MakeLarger();
	font.MakeLarger();
	this->SetFont(font);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	//wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, wxSize(1000, 1500));

	this->SetBackgroundColour(wxColor("#313947"));
	wxInitAllImageHandlers();
	panel = new wxImagePanel(this, "assets/final/" + assetsFolder + "/board.png", wxBITMAP_TYPE_PNG, margin);
	
	//sizer->Add(panel, 0, wxLEFT | wxTOP, margin);
	
	auto uiPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition);
	uiPanel->SetBackgroundColour(wxColor("#bdb295"));
	
	mainSizer->Add(panel, 2, wxEXPAND | wxTOP | wxLEFT, margin);
	mainSizer->Add(uiPanel, 1, wxEXPAND | wxALL, margin);
	
	wxBoxSizer* uiSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* whoseTurnText = new wxStaticText(uiPanel, wxID_ANY, "Whose turn:");
	whoseTurnText->SetForegroundColour(wxColor("#f4f3f1"));
	uiSizer->Add(whoseTurnText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);
	whoseTurnTextUnder = new wxStaticText(uiPanel, wxID_ANY, "");
	uiSizer->Add(whoseTurnTextUnder, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);

	wxStaticText* whoWinsText = new wxStaticText(uiPanel, wxID_ANY, "Who is winning?");
	whoWinsText->SetForegroundColour(wxColor("#f4f3f1"));
	uiSizer->Add(whoWinsText, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);
	whoWinsTextUnder = new wxStaticText(uiPanel, wxID_ANY, "");
	uiSizer->Add(whoWinsTextUnder, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);

	wxButton* newGameButton = new wxButton(uiPanel, wxID_ANY, "New game");
	newGameButton->SetForegroundColour(wxColor("#f4f3f1"));
	newGameButton->SetBackgroundColour(wxColor("#313947"));
	newGameButton->Bind(wxEVT_BUTTON, &MainFrame::startNewGame, mainFramePointer);
	uiSizer->Add(newGameButton, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);

	// wxButton* surrenderButton = new wxButton(uiPanel, wxID_ANY, "Surrender");
	// surrenderButton->Bind(wxEVT_BUTTON, &MainFrame::surrenderGame, mainFramePointer);
	// uiSizer->Add(surrenderButton, 0, wxALIGN_CENTER_HORIZONTAL);

	wxButton* infoButton = new wxButton(uiPanel, wxID_ANY, "Info");
	infoButton->SetForegroundColour(wxColor("#f4f3f1"));
	infoButton->SetBackgroundColour(wxColor("#313947"));
	infoButton->Bind(wxEVT_BUTTON, &MainFrame::displayInfo, mainFramePointer);
	uiSizer->Add(infoButton, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);
	
	wxButton* quitButton = new wxButton(uiPanel, wxID_ANY, "Quit");
	quitButton->SetForegroundColour(wxColor("#f4f3f1"));
	quitButton->SetBackgroundColour(wxColor("#313947"));
	quitButton->Bind(wxEVT_BUTTON, &MainFrame::quitApp, mainFramePointer);
	uiSizer->Add(quitButton, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);

	uiPanel->SetSizer(uiSizer);
	this->SetSizer(mainSizer);

	newGame();
}

void MainFrame::checkBoard() {
	int moveId = 0;
	int whiteBeated = 0;
	int blackBeated = 0;
	int destRow;
	int destCol;
	maxPawnsToBeat = 0;

	for (int i = 0; i < 24; i++) {
		Pawn* pawn = pawns[i];
		if (!pawn->isOnBoard) {
			if (pawn->color == WHITE) {
				whiteBeated += 1;
			} else {
				blackBeated += 1;
			}
		} else if (pawn->color == whoseTurn){
			std::set<Pawn*> pawnsToBeat = {};
			checkForMove(pawn->row, pawn->col, pawn, NONE, pawnsToBeat);
		}
	}

	wxString text;
	if (whiteBeated == blackBeated) {
		text = "No one it's a draw";
		whoWinsTextUnder->SetForegroundColour(wxColor("#0fff4f"));
	} else if (whiteBeated > blackBeated) {
		text.append("Black by ");
		text.append(std::to_string(whiteBeated - blackBeated));
		text.append(whiteBeated - blackBeated == 1 ? " piece" : " pieces");
		whoWinsTextUnder->SetForegroundColour(wxColor("#2f2d2d"));
	} else {
		text.append("White by ");
		text.append(std::to_string(blackBeated - whiteBeated));
		text.append(blackBeated - whiteBeated == 1 ? " piece" : " pieces");
		whoWinsTextUnder->SetForegroundColour(wxColor("#fbeca6"));
	}
	whoWinsTextUnder->SetLabelText(text);
	whoseTurnTextUnder->SetLabelText(whoseTurn == WHITE ? "White's" : "Black's");
	whoseTurnTextUnder->SetForegroundColour(wxColor(whoseTurn == WHITE ? "#fbeca6" : "#2f2d2d"));

	if (pawnMoves.size() == 0) {
		wxString whoWonText;
		whoWonText.append(whoseTurn == WHITE ? "Black " : "White ");
		whoWonText.append("won the game!");
		whoseTurnTextUnder->SetForegroundColour(wxColor("#fc2819"));
		whoseTurnTextUnder->SetLabelText("No one's");
		wxMessageBox(whoWonText, "GG!");
	} 


}

void MainFrame::onPawnClick(wxCommandEvent& evt) {
	clearPawnMoveButtons();
	
	int pawnButtonId = evt.GetId();
	Pawn* pawn = pawns[pawnButtonId - 1000];
	pawnToMove = nullptr;

	if (pawn->isMovable) {
		pawnToMove = pawn;
		// MoveType wantedType;

		// if (isAnyBeatMove)
		// 	wantedType = BEAT;
		// else
		// 	wantedType = MOVE;

		for (int i = 0; i < pawnMoves.size(); i++) {
			auto iter = std::next(pawnMoves.begin(), i);
			PawnMove* pawnMove = *iter;

			if (pawnMove->pawn == pawn ) {
				if (pawnMove->pawnsToBeat.size() == maxPawnsToBeat) {
					createPawnMoveButton(pawnMove);
				}
			}
		}
	}
}

void MainFrame::onPawnMoveClick(wxCommandEvent& evt) {
	//wxLogMessage(wxString::Format("%d", pawnToMove->pawnButton->GetId()));

	auto iter = std::next(pawnMoves.begin(), evt.GetId());
	PawnMove* move = *iter;

	if (move->moveType == MOVE) {
		board[pawnToMove->row][pawnToMove->col]->erasePawn();
		board[move->row][move->col]->putPawn(pawnToMove);
		pawnToMove->pawnButton->SetPosition(buttonPoint(move->row, move->col));
	}
	else {
		board[pawnToMove->row][pawnToMove->col]->erasePawn();
		for (Pawn* pawn: move->pawnsToBeat) {
			beatPawn(board[pawn->row][pawn->col]->pawn);
			board[pawn->row][pawn->col]->erasePawn();
		}
		board[move->row][move->col]->putPawn(pawnToMove);
		pawnToMove->pawnButton->SetPosition(buttonPoint(move->row, move->col));
	}
	
	clearPawnMoveButtons();
	Pawn* pawn = move->pawn;
	if (move->row == 0 && pawn->color == WHITE && pawn->isQueen == false) {
		pawn->pawnButton->SetBitmap(wxBitmap(wxImage("assets/final/" + assetsFolder + "/white_queen.png", wxBITMAP_TYPE_PNG)));
		pawn->isQueen = true;
	} 
	if (move->row == 7 && pawn->color == BLACK && pawn->isQueen == false) {
		pawn->pawnButton->SetBitmap(wxBitmap(wxImage("assets/final/" + assetsFolder + "/black_queen.png", wxBITMAP_TYPE_PNG)));
		pawn->isQueen = true;
	} 

	isAnyBeatMove = false;
	pawnMoves.clear();
	whoseTurn = whoseTurn == WHITE ? BLACK : WHITE;
	id = 0;

	checkBoard();
}

void MainFrame::createPawnMoveButton(PawnMove* move) {
	int red = 0;
	int green = 0;

	if (move->moveType == MOVE)
		green = 255;
	else
		red = 255;

	move->moveButton = new wxButton(panel, move->id, "", buttonPoint(move->row, move->col), buttonSize, wxBORDER_NONE);
	move->moveButton->SetBackgroundColour(wxColor(red, green, 0));
	move->moveButton->Bind(wxEVT_BUTTON, &MainFrame::onPawnMoveClick, mainFramePointer);
}

void MainFrame::createPawnButton(Pawn* pawn) {
	wxString imageSrc;

	if (pawn->color == BLACK)
		imageSrc = "assets/final/" + assetsFolder + "/black_pawn.png";
	else
		imageSrc = "assets/final/" + assetsFolder + "/white_pawn.png";

	pawn->pawnButton = new wxBitmapButton(panel, 1000 + id, wxBitmap(wxImage(imageSrc, wxBITMAP_TYPE_PNG)),
		buttonPoint(pawn->row, pawn->col), buttonSize, wxBORDER_NONE);
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

void MainFrame::createPawnMove(int row, int col, Pawn* pawn, int moveId, MoveType moveType, std::set<Pawn*> pawnsToBeat) {
	PawnMove* pawnMove = new PawnMove(row, col, pawn, moveType, moveId);
	pawnMove->moveButton = nullptr;
	pawnMove->pawnsToBeat = pawnsToBeat;
	pawn->isMovable = true;
	pawnMoves.push_back(pawnMove);
	//change the place of incrementation of moveId variable
	//moveId++;
}

void MainFrame::putPawnsOnBoard() {
	int index = 0;
	Color color = BLACK;

	for (int o = 0; o <= 1; o++) {
		for (int i = o * 5; i < o * 5 + 3; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j]->color != BLACK)
					continue;

				pawns[index] = new Pawn(color, i, j, false);
				pawns[index]->isOnBoard = true;
				createPawnButton(pawns[index]);
				board[i][j]->putPawn(pawns[index]);
				pawns[index]->pawnButton->Bind(wxEVT_BUTTON, &MainFrame::onPawnClick, this);
				index++;
			}
		}
		color = WHITE;
	}
		// pawns[12].pawnButton->SetBitmap(wxBitmap(wxImage("assets/experimental/white_queen.png", wxBITMAP_TYPE_PNG)));
		// pawns[12].isQueen = true;
		// pawns[11].pawnButton->SetBitmap(wxBitmap(wxImage("assets/experimental/black_queen.png", wxBITMAP_TYPE_PNG)));
		// pawns[11].isQueen = true; 
}

//null direction will result in less code in function searvhing for all moves
void MainFrame::checkForMove(int row, int col, Pawn* pawn, Direction directionToErase, std::set<Pawn*> pawnsToBeat, Direction queenDirection) {
	std::set <Direction> directionsWithoutBeat = {};
	std::set<Direction> directions = {BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT};
	std::set<Pawn*> pawnsToBeatLocal;
	directions.erase(directionToErase);
	if (pawnsToBeat.size() == 0) {
		if (pawn->isQueen) {
			directionsWithoutBeat = {BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT};
		} else {
			if (pawn->color == WHITE) {
				directionsWithoutBeat.insert(TOP_LEFT);
				directionsWithoutBeat.insert(TOP_RIGHT);
			} else {
				directionsWithoutBeat.insert(BOTTOM_LEFT);
				directionsWithoutBeat.insert(BOTTOM_RIGHT);
			}
		}
		if (queenDirection != NONE) {
			directionsWithoutBeat = {queenDirection};
			directions = {queenDirection};
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
			if (field->pawn != nullptr ) {
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
			} else if (directionsWithoutBeat.count(direction) == 1) {
				createPawnMove(destRow, destCol, pawn, pawnMoves.size(), MOVE);
				if (pawn->isQueen) {
					checkForMove(destRow, destCol, pawn, getCounterDirection(direction), pawnsToBeatLocal, direction);
				} 
			}
		}
	}
	if (numberOfPawnsToBeatAtBeginning == pawnsToBeat.size() && pawnsToBeat.size() != 0) {
		createPawnMove(row, col, pawn, pawnMoves.size(), BEAT, pawnsToBeat);
		if (pawnsToBeat.size() > maxPawnsToBeat) {
			maxPawnsToBeat = pawnsToBeat.size();
		}
	}
}

void MainFrame::beatPawn(Pawn* pawn) {
	pawn->isOnBoard = false;
	pawn->pawnButton->Destroy();
	pawn->pawnButton = nullptr;
}

void MainFrame::incrementDestinationCoordinates(Direction direction, int& row, int& col) {
	switch (direction) {
				case TOP_LEFT:
					row -= 1;
					col -= 1;
					break;
				case TOP_RIGHT:
					row -= 1;
					col += 1;
					break;
				case BOTTOM_LEFT:
					row += 1;
					col -= 1;
					break;
				case BOTTOM_RIGHT:
					row += 1;
					col += 1;
					break;
			}
}

Direction MainFrame::getCounterDirection(Direction direction) {
	Direction counterDirection;
	switch(direction) {
		case TOP_LEFT:
			counterDirection = BOTTOM_RIGHT;
			break;
		case TOP_RIGHT:
			counterDirection = BOTTOM_LEFT;
			break;
		case BOTTOM_LEFT:
			counterDirection = TOP_RIGHT;
			break;
		case BOTTOM_RIGHT:
			counterDirection = TOP_LEFT;
			break;
	}
	return counterDirection;
}

wxPoint MainFrame::buttonPoint(int row, int col) {
	return wxPoint((col * ceil(1.2 * SIZE)) + (SIZE * 0.1), (row * ceil(1.2 * SIZE)) + (SIZE * 0.1));
}

void MainFrame::startNewGame(wxCommandEvent& evt) {
	auto result = wxMessageBox(
		"Are you sure that you want to start a new game?\nThis means that you will lose your actual game!",
	 	"Starting a new game", wxYES_NO);

	switch (result) {
		case (wxYES):
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					delete board[i][j];
				}
			}
			for (Pawn* pawn : pawns) {
				if (pawn->pawnButton != nullptr) {
					pawn->pawnButton->Destroy();
				}
				delete pawn;
			}
			clearPawnMoveButtons();
			newGame();
			break;
	}
}

void MainFrame::surrenderGame(wxCommandEvent& evt) {
	
}

void MainFrame::displayInfo(wxCommandEvent& evt) {
	wxMessageBox("Name: Checkers\nAuthor: Kamil Kowalczyk", "Information about program");
}

void MainFrame::newGame() {
	pawnToMove = nullptr;
	bool isAnyBeatMove = false;
	int maxPawnsToBeat = 0;
	whoseTurn = WHITE;
	id = 0;

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

void MainFrame::quitApp(wxCommandEvent& evt) {
	auto result = wxMessageBox(
		"Are you sure that you want to quit the game?\nThis means that you will lose your actual game!",
	 	"Qutting the game", wxYES_NO);

	if (result == wxYES) {
		mainFramePointer->Close();
	}
}