#pragma once

#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/display.h>

class App : public wxApp
{
public:
	/*
	* A method for showing the MainFrame where the entire UI is coded
	*/
	bool OnInit();
};

