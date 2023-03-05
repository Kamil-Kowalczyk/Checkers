#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("Checkers", wxDEFAULT_FRAME_STYLE);
	//mainFrame->ShowFullScreen(true);
	//mainFrame->Maximize();
	mainFrame->SetMinClientSize(wxSize(600, 650));
	mainFrame->CenterOnScreen();
	//mainFrame->EnableMaximizeButton(false);
	//mainFrame->SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
	//mainFrame->Maximize(true);
	mainFrame->Show();
	return true;
}