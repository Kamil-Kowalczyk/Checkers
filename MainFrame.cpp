#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const long& style) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style) {
	mainFramePointer = this;

	wxDisplay* display = new wxDisplay(wxDisplay::GetFromWindow(mainFramePointer));
	wxRect* screen = new wxRect(display->GetClientArea());
	if (screen->height > 1600) {
		SCALE = 2;
		PIXEL_SIZE_OF_CELL = 240;
		PIXEL_SIZE_OF_PAWN = 200;
		assetsFolder = "x2.0";
	} else if (screen->height > 1000) {
		SCALE = 1;
		PIXEL_SIZE_OF_CELL = 120;
		PIXEL_SIZE_OF_PAWN = 100;
		assetsFolder = "x1.0";
	} else {
		SCALE = 0.5;
		PIXEL_SIZE_OF_CELL = 80;
		PIXEL_SIZE_OF_PAWN = 66;
		assetsFolder = "x0.5";
	}
	calculateButtonOffset();
	delete display, screen;
	buttonSize = wxSize(PIXEL_SIZE_OF_PAWN, PIXEL_SIZE_OF_PAWN);
	margin = SCALE * 20;

	auto font = this->GetFont();
	font.MakeLarger();
	font.MakeLarger();
	this->SetFont(font);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	this->SetBackgroundColour(wxColor("#313947"));
	wxInitAllImageHandlers();
	panel = new wxImagePanel(this, "assets/final/" + assetsFolder + "/board.png", wxBITMAP_TYPE_PNG, margin);
	
	
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

	wxButton* newGameButton = new wxButton(uiPanel, wxID_ANY, "New game", wxDefaultPosition,  wxSize(100, 50));
	newGameButton->SetForegroundColour(wxColor("#2f2d2d"));
	newGameButton->SetBackgroundColour(wxColor("#e2e1e1"));
	newGameButton->Bind(wxEVT_BUTTON, &MainFrame::startNewGame, mainFramePointer);
	uiSizer->Add(newGameButton, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);

	wxButton* infoButton = new wxButton(uiPanel, wxID_ANY, "Info");
	infoButton->SetForegroundColour(wxColor("#2f2d2d"));
	infoButton->SetBackgroundColour(wxColor("#e2e1e1"));
	infoButton->Bind(wxEVT_BUTTON, &MainFrame::displayInfo, mainFramePointer);
	uiSizer->Add(infoButton, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);
	
	wxButton* quitButton = new wxButton(uiPanel, wxID_ANY, "Quit");
	quitButton->SetForegroundColour(wxColor("#2f2d2d"));
	quitButton->SetBackgroundColour(wxColor("#e2e1e1"));
	quitButton->Bind(wxEVT_BUTTON, &MainFrame::quitApp, mainFramePointer);
	uiSizer->Add(quitButton, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, margin);

	uiPanel->SetSizer(uiSizer);
	this->SetSizer(mainSizer);

	this->game = new Game();
	generatePawnsOnBoard();
	updateTexts();
}

void MainFrame::onPawnClick(wxCommandEvent& evt) {
	clearPawnMoveButtons();
	if (game->pawnToMove != nullptr)
		game->pawnToMove->pawnButton->SetBackgroundColour(wxColor("#6b3511"));
	int pawnId = evt.GetId() - 1000;
	std::list<PawnMove*> pawnMoves = game->handlePawnClick(pawnId);
	Pawn* clickedPawn = game->getPawnById(pawnId);
	if (clickedPawn->color == game->whoseTurn) {
		if (pawnMoves.size() == 0) {
			wxString messageText = "This pawn can't be moved as it doesn't have any available moves";
			if (game->isAnyCapturingMove)
				messageText = "This pawn can't be moved as there are pawns with available capturing moves";
			wxMessageBox(messageText, "Information about available moves");
		}
		else {
			clickedPawn->pawnButton->SetBackgroundColour(wxColor("#895d41"));
			for (PawnMove* move : pawnMoves) {
				createPawnMoveButton(move);
			}
		}
	}
	else {
		wxMessageBox(wxString("It is ").Append(game->whoseTurn == WHITE ? "White's " : "Black's ").Append("turn!"), "Information about the actual player");
	}
	
}

void MainFrame::onPawnMoveClick(wxCommandEvent& evt) {
	bool hasBecameQueen = game->performMove(evt.GetId());
	Pawn* pawnToMove = game->pawnToMove;
	pawnToMove->pawnButton->SetPosition(buttonPoint(pawnToMove->row, pawnToMove->col));
	if (pawnToMove != nullptr)
		pawnToMove->pawnButton->SetBackgroundColour(wxColor("#6b3511"));
	if (hasBecameQueen) {
		if (pawnToMove->color == WHITE)
			pawnToMove->pawnButton->SetBitmap(wxBitmap(wxImage("assets/final/" + assetsFolder + "/white_queen.png", wxBITMAP_TYPE_PNG)));
		else 
			pawnToMove->pawnButton->SetBitmap(wxBitmap(wxImage("assets/final/" + assetsFolder + "/black_queen.png", wxBITMAP_TYPE_PNG)));
	}

	clearPawnMoveButtons();

	game->pawnMoves.clear();

	game->checkBoard();
	updateTexts();
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

void MainFrame::createPawnButton(Pawn* pawn, int pawnId) {
	wxString imageSrc;

	if (pawn->color == BLACK)
		imageSrc = "assets/final/" + assetsFolder + "/black_pawn.png";
	else
		imageSrc = "assets/final/" + assetsFolder + "/white_pawn.png";

	pawn->pawnButton = new wxBitmapButton(panel, 1000 + pawnId, wxBitmap(wxImage(imageSrc, wxBITMAP_TYPE_PNG)),
		buttonPoint(pawn->row, pawn->col), buttonSize, wxBORDER_NONE);
	pawn->pawnButton->SetBackgroundColour(wxColor(107, 53, 17));
}

wxPoint MainFrame::buttonPoint(int row, int col) {
	return wxPoint((col * PIXEL_SIZE_OF_CELL + PIXEL_OFFSET_OF_BUTTON_POINT), (row * PIXEL_SIZE_OF_CELL + PIXEL_OFFSET_OF_BUTTON_POINT));
}

void MainFrame::startNewGame(wxCommandEvent& evt) {
	auto result = wxMessageBox(
		"Are you sure that you want to start a new game?\nThis means that you will lose your actual game!",
	 	"Starting a new game", wxYES_NO);

	switch (result) {
		case (wxYES):
			game->newGame();
			generatePawnsOnBoard();
			updateTexts();
			break;
	}
}

void MainFrame::displayInfo(wxCommandEvent& evt) {
	wxMessageBox("Project Name: Checkers\nAuthor: Kamil Kowalczyk Wydzial MS, Informatyka profil praktyczny, semestr 1, grupa lab. 1.1\n Crown icons created by smashingstocks - Flaticon https://www.flaticon.com/free-icons/crown", "Information about program");
}

void MainFrame::quitApp(wxCommandEvent& evt) {
	auto result = wxMessageBox(
		"Are you sure that you want to quit the game?\nThis means that you will lose your actual game!",
	 	"Quitting the game", wxYES_NO);

	if (result == wxYES) {
		mainFramePointer->Close();
	}
}

void MainFrame::calculateButtonOffset() {
	PIXEL_OFFSET_OF_BUTTON_POINT = (PIXEL_SIZE_OF_CELL - PIXEL_SIZE_OF_PAWN) / 2;
}

void MainFrame::generatePawnsOnBoard() {
	int pawnId = 0;
	for (Pawn* pawn : game->pawns) {
		createPawnButton(pawn, pawnId);
		pawn->pawnButton->Bind(wxEVT_BUTTON, &MainFrame::onPawnClick, this);
		pawnId++;
	}
}

void MainFrame::updateTexts() {
	int whiteCaptured = game->whiteCaptured;
	int blackCaptured = game->blackCaptured;
	wxString text;

	if (whiteCaptured == blackCaptured) {
		text = "No one it's a draw";
		whoWinsTextUnder->SetForegroundColour(wxColor("#0fff4f"));
	}
	else if (whiteCaptured > blackCaptured) {
		text.append("Black by ");
		text.append(std::to_string(whiteCaptured - blackCaptured));
		text.append(whiteCaptured - blackCaptured == 1 ? " piece" : " pieces");
		whoWinsTextUnder->SetForegroundColour(wxColor("#2f2d2d"));
	}
	else {
		text.append("White by ");
		text.append(std::to_string(blackCaptured - whiteCaptured));
		text.append(blackCaptured - whiteCaptured == 1 ? " piece" : " pieces");
		whoWinsTextUnder->SetForegroundColour(wxColor("#fbeca6"));
	}
	whoWinsTextUnder->SetLabelText(text);
	whoseTurnTextUnder->SetLabelText(game->whoseTurn == WHITE ? "White's" : "Black's");
	whoseTurnTextUnder->SetForegroundColour(wxColor(game->whoseTurn == WHITE ? "#fbeca6" : "#2f2d2d"));

	if (game->pawnMoves.size() == 0) {
		wxString whoWonText;
		whoWonText.append(game->whoseTurn == WHITE ? "Black " : "White ");
		whoWonText.append("won the game!");
		whoseTurnTextUnder->SetForegroundColour(wxColor("#fc2819"));
		whoseTurnTextUnder->SetLabelText("No one's");
		wxMessageBox(whoWonText, "GG!");
	}
}

void MainFrame::clearPawnMoveButtons() {
	PawnMove* pawnMove;
	int size = game->pawnMoves.size();
	for (int i = 0; i < size; i++) {
		auto iter = std::next(game->pawnMoves.begin(), i);
		pawnMove = *iter;
		if (pawnMove->moveButton != nullptr) {
			pawnMove->moveButton->Destroy();
			pawnMove->moveButton = nullptr;
		}
	}
}