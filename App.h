#pragma once

#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/display.h>

class App : public wxApp
{
public:
	/*
	a method for showing the MainFrame where the entire logic and view is coded
	*/
	bool OnInit();
};

