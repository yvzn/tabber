#include "ChordsToolbar.h"
#include "../ui/ChordsTabControl.h"
#include "../ui/MainWindow.h"

const int ChordsToolbar::CHORD_BUTTON_HEIGHT = 25;
const int ChordsToolbar::CHORD_BUTTON_WIDTH = 50;
const int ChordsToolbar::CHORD_BUTTON_PADDING = 2;


ChordsToolbar::ChordsToolbar(ChordsTabControl* parent)
	: ChordsTabControlPanel(parent)
{
	OBJECT_CREATED;
}


ChordsToolbar::~ChordsToolbar()
{
	OBJECT_DELETED;
}


void ChordsToolbar::create(HWND hParentWindow)
{
	try
	{
		ChordsTabControlPanel::create(hParentWindow);
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("ChordsToolbar::create", ex);
	}

	_xNextButtonPosition = 0;
}


void ChordsToolbar::addButton(const char* buttonLabel, int buttonId)
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
		throw new RuntimeException("ChordsToolbar::addButton", "Could not create chord button");
	}

	//subclassing (define my own window proc for this control)
	_superClassWindowProc = (WNDPROC)SetWindowLong(hButton, GWL_WNDPROC, (long)ChordsTabControlPanel::forwardMessage);

    //store *this pointer in window handle so that I can access class members later
    SetProp(hButton, "CorrespondingObject", (void*)this);

	ApplyUsersDefaultFont(hButton);
	
	_xNextButtonPosition += CHORD_BUTTON_WIDTH + CHORD_BUTTON_PADDING;
}


/**
 * @see MainWindow::handleMessage
 */
LRESULT CALLBACK ChordsToolbar::handleMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
	if(message == WM_LBUTTONUP)
	{
		unsigned int commandId = GetDlgCtrlID(hWindow);
		EditArea* editArea = _parent->getMainWindow()->getEditArea();

		if(wParam & MK_CONTROL)
		{
			editArea->onInsertChord(commandId, ArpeggioDispatcher::ASCENDING);
		}
		else if(wParam & MK_SHIFT)
		{
			editArea->onInsertChord(commandId, ArpeggioDispatcher::DESCENDING);
		}
 		else
		{
   			editArea->onInsertChord(commandId);
		}
	}

	return CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);
}


