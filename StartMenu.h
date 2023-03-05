#pragma once

#include <wx/wx.h>

class StartMenu
{
public:
	wxPanel* panel;
	wxFrame* parent;
	wxButton* startGameButton;
	wxButton* quitButton;
	wxStaticText* title;

	int screenWidth, screenHeight;

	StartMenu(wxFrame* parent, int width, int height);
	~StartMenu();

	wxPoint getCentredPoint(wxSize size);
	void setSizeAndCentredPosition(wxControl* entity, wxSize size);
};

