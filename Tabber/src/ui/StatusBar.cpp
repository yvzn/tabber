#include "StatusBar.h"


StatusBar::StatusBar()
{
    _typingModeString = new char[4];
	OBJECT_CREATED;
}


StatusBar::~StatusBar()
{
    delete [] _typingModeString;
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

    int partsRightBorderPositions[] = {30, 60, -1};
    SendMessage(_hWindow, SB_SETPARTS, sizeof(partsRightBorderPositions)/sizeof(int), (LPARAM)partsRightBorderPositions);
}


void StatusBar::resize()
{
	assert(_hWindow != NULL);
	//StatusBar can resize itself automatically inside parent window
	SendMessage(_hWindow, WM_SIZE, 0, 0);
}


RECT StatusBar::getSize()
{
	assert(_hWindow != NULL);
	
	RECT windowRect;
	GetWindowRect(_hWindow, &windowRect);
	
	return windowRect;
}


void StatusBar::setTextInPart(int indexOfPart, const char* newPartText)
{
	assert(_hWindow != NULL);
    PostMessage(_hWindow, SB_SETTEXT, (WPARAM)indexOfPart, (LPARAM)newPartText);
}


void StatusBar::updateTypingMode(TypingMode typingMode)
{
	switch(typingMode)
	{
		case TYPING_OVERWRITE:
      	{
			lstrcpy(_typingModeString, "OVR");
			break;
		}

		case TYPING_SPECIAL:
      	{
			lstrcpy(_typingModeString, "SPL");
			break;
		}

		default:
      	{
			lstrcpy(_typingModeString, "INS");
			break;
		}
	}
	setTextInPart(0, _typingModeString);
}

