#include "ChordButtonsPanel.h"
#include "../ui/MainWindow.h"

const int ChordButtonsPanel::CHORD_BUTTON_HEIGHT = 25;
const int ChordButtonsPanel::CHORD_BUTTON_WIDTH = 50;
const int ChordButtonsPanel::CHORD_BUTTON_PADDING = 2;


ChordButtonsPanel::ChordButtonsPanel()
{
	OBJECT_CREATED;
}


ChordButtonsPanel::~ChordButtonsPanel()
{
	OBJECT_DELETED;
}


void ChordButtonsPanel::create(HWND hParentWindow)
{
	_hWindow = CreateWindowEx(
		0,
		"STATIC",
		NULL,
		WS_CHILD,
		CW_USEDEFAULT,
  		CW_USEDEFAULT,
    	CW_USEDEFAULT,
     	CW_USEDEFAULT,
		hParentWindow,
  		NULL,
		GetModuleHandle(NULL),
  		(void*)this );
		
	if(_hWindow == NULL)
	{
		throw new RuntimeException("ChordButtonsPanel::create", "Could not create buttons panel");			
	}

	//subclassing (define my own window proc for this control)
	_superClassWindowProc = (WNDPROC)SetWindowLong(_hWindow, GWL_WNDPROC, (long)ChordButtonsPanel::forwardMessage);
    
    //store *this pointer in window handle so that I can access class variables and methods
    SetProp(_hWindow, "CorrespondingObject", (void*)this);
    
	_xNextButtonPosition = 0;
}


void ChordButtonsPanel::addButton(const char* buttonLabel, int buttonId)
{
	assert(_hWindow != NULL);

	HWND hButton = CreateWindowEx(
		0,
		"BUTTON",
		buttonLabel,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		_xNextButtonPosition,
		CHORD_BUTTON_PADDING,
		CHORD_BUTTON_WIDTH,
		CHORD_BUTTON_HEIGHT,
		_hWindow,
		(HMENU)buttonId,
   		GetModuleHandle(NULL),
  		NULL );
  		
	if(hButton == NULL)
	{
		throw new RuntimeException("ChordsToolbar::createChordButton", "Could not create chord button");			
	}

	ApplyUsersDefaultFont(hButton);
	
	_xNextButtonPosition += CHORD_BUTTON_WIDTH + CHORD_BUTTON_PADDING;
}


void ChordButtonsPanel::show()
{
	assert(_hWindow != NULL);
	ShowWindow(_hWindow, SW_SHOW);
}


void ChordButtonsPanel::hide()
{
	assert(_hWindow != NULL);
	ShowWindow(_hWindow, SW_HIDE);
}


/*
 * Used by parent window to resize this panel.
 * Parent window is in charge of resizing because this panel is designed to be
 * included in a multi-part window (e.g. a tab control)
*/
HWND& ChordButtonsPanel::getWindowHandle()
{
	return _hWindow;
}


/**
 * @see MainWindow::forwardMessage
 */
LRESULT CALLBACK ChordButtonsPanel::forwardMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
	//retrieve *this pointer and then forward message
    ChordButtonsPanel* panel = (ChordButtonsPanel*)GetProp(hWindow, "CorrespondingObject");
    if (panel)
    {
        return panel->handleMessage(hWindow, message, wParam, lParam);
	}
    else
    {
        return DefWindowProc(hWindow, message, wParam, lParam);
    }
}


/**
 * @see MainWindow::handleMessage
 */
LRESULT CALLBACK ChordButtonsPanel::handleMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
    if(message == WM_COMMAND && LOWORD(wParam) >= IDC_FIRST_CHORD)
    {
		DebugWindow::trace("%d", LOWORD(wParam));
	}
	else
	{
		return CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);
	}
    return 0;
}


