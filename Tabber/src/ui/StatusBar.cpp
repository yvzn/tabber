#include "StatusBar.h"


StatusBar::StatusBar()
{
	OBJECT_CREATED;
}


StatusBar::~StatusBar()
{
	OBJECT_DELETED;
}


void StatusBar::create(HWND hParentWindow)
{
	_hWindow = CreateWindowEx (
		0,
		STATUSCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		0, 0, 0, 0,
		hParentWindow, (HMENU)IDC_MAIN_STATUSBAR, GetModuleHandle(NULL), NULL );

	if(_hWindow == NULL)
	{
		throw new RuntimeException("StatusBar::create", "Could not create main status bar");	
	}

    int partWidths[] = {-1};
    SendMessage(_hWindow, SB_SETPARTS, sizeof(partWidths)/sizeof(int), (LPARAM)partWidths);
}


void StatusBar::resize()
{
	assert(_hWindow != NULL);
	//StatusBar can resize itself automatically inside parent window
	SendMessage(_hWindow, WM_SIZE, 0, 0);
}


void StatusBar::setTextInPart(int indexOfPart, const char* newPartText)
{
	assert(_hWindow != NULL);
    PostMessage(_hWindow, SB_SETTEXT, (WPARAM)indexOfPart, (LPARAM)newPartText);
}


RECT StatusBar::getSize()
{
	assert(_hWindow != NULL);
	
	RECT windowRect;
	GetWindowRect(_hWindow, &windowRect);
	
	return windowRect;
}

