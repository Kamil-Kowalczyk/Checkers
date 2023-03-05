#include "StartMenu.h"

StartMenu::StartMenu(wxFrame* parent, int width, int height) {
	this->screenWidth = width;
	this->screenHeight = height;

	this->parent = parent;
	this->panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(width, height));
	panel->SetBackgroundColour(wxColor(27, 134, 180));
	

	//wxPoint position = wxPoint(width / 2 - title->GetSize().x / 2, 0.4 * title->GetSize().x);

	//wxFont myFont = wxFont((wxFont) wxNORMAL_FONT);

	title = new wxStaticText(panel, wxID_ANY, "Checkers", wxDefaultPosition, wxSize(500, 600), wxALIGN_CENTRE_HORIZONTAL);
	title->SetBackgroundColour(wxColor(255,0,0));
	wxFont font = title->GetFont();
	font.SetPixelSize(wxSize(0,80));
	title->SetFont(font);
	//title->SetSize(wxSize(wxSIZE_AUTO_WIDTH, wxSIZE_AUTO_HEIGHT));
	//setSizeAndCentredPosition(title, wxSize(title->GetMaxWidth(), 80));
	//title->SetFont(title->GetFont().Scale(5.5).MakeBold());

	// startGameButton = new wxButton(panel, wxID_ANY, "New game");
	// startGameButton->SetPosition(wxPoint(width / 2 - startGameButton->GetSize().x / 2, title->GetPosition().y + 1.2 * title->GetSize().y));

	// quitButton = new wxButton(panel, wxID_ANY, "Quit");
	// quitButton->SetSize(startGameButton->GetSize());
	// quitButton->SetPosition(wxPoint(startGameButton->GetPosition().x, 1.1 * startGameButton->GetPosition().y + startGameButton->GetSize().y));
	
}

void StartMenu::setSizeAndCentredPosition(wxControl* entity, wxSize size) {
	//entity->SetSize(size);
	wxPoint point = getCentredPoint(entity->GetSize());
	entity->SetPosition(point);
}

wxPoint StartMenu::getCentredPoint(wxSize size) {
	return wxPoint(screenWidth / 2 - size.x / 2, 0.4 * size.x);
}