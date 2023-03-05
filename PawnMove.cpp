#include "PawnMove.h"

PawnMove::PawnMove(int row, int col, int size, MoveType moveType, int id, int rowToBeat, int colToBeat) {
	this->row = row;
	this->col = col;
	this->size = size;
	this->moveType = moveType;
	this->id = id;
	this->rowToBeat = rowToBeat;
	this->colToBeat = colToBeat;
	//this->mainFramePointer = mainFramePointer;
}
PawnMove::~PawnMove() {

}
//void PawnMove::createButton() {
//	if (moveType == MOVE) {
//		moveButton = new wxButton(context, id, "", wxPoint(col * size + (size * 0.2), row * size + (size * 0.2)),
//			wxSize(0.6 * size, 0.6 * size), wxBORDER_NONE);
//		moveButton->SetBackgroundColour(wxColor(0, 255, 0));
//		moveButton->Bind(wxEVT_BUTTON, &MainFrame::onMoveClick, mainFramePointer);
//	}
//	else {
//
//	}
//}