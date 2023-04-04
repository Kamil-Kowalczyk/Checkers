#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("Checkers", wxDEFAULT_FRAME_STYLE);
	//mainFrame->SetMinClientSize(wxSize(600, 650));
	mainFrame->ShowFullScreen(true);
	//mainFrame->CenterOnScreen();
	mainFrame->Show();
	return true;
}