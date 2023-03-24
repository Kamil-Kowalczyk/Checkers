#include "Pawn.h"

//todo construcor without parameters
Pawn::Pawn() {
	/*this->color = NONE;
	this->col = -1;
	this->row = -1;
	this->onBoard = false;*/
}
Pawn::Pawn(Color color, int row, int col, bool onBoard) {
	this->color = color;
	this->col = col;
	this->row = row;
	this->isOnBoard = onBoard;
	this->isMovable = false;
	this->isQueen = false;
	//this->context = context;

	//createButton();
}
Pawn::~Pawn(){}
//void Pawn::createButton() {
//	wxString imageSrc;
//	if (color == BLACK)
//		imageSrc = "assets/black_pawn.png";
//	else
//		imageSrc = "assets/white_pawn.png";
//
//	pawnButton = new wxBitmapButton(context, 1000 + id, wxBitmap(wxImage(imageSrc, wxBITMAP_TYPE_PNG)),
//		wxPoint(col * size + (size * 0.2), row * size + (size * 0.2)), wxSize(0.6 * size, 0.6 * size), wxBORDER_NONE);
//	pawnButton->SetBackgroundColour(wxColor(107, 53, 17));
//	id++;
//}
