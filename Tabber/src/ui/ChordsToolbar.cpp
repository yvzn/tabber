#include "ChordsToolbar.h"
#include "../ui/ChordsTabControl.h"
#include "../ui/MainWindow.h"


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
}


HWND ChordsToolbar::addButton(const char* buttonLabel, int buttonId, DWORD buttonStyle)
{
	assert(_hWindow != NULL);

	HWND hButton = ChordsTabControlPanel::addButton(buttonLabel, buttonId, buttonStyle | BS_PUSHBUTTON);

	//subclassing (define my own window proc for this control)
	_superClassWindowProc = (WNDPROC)SetWindowLong(hButton, GWL_WNDPROC, (long)ChordsTabControlPanel::forwardMessage);

    //store *this pointer in window handle so that I can access class members later
    SetProp(hButton, "CorrespondingObject", (void*)this);

	return hButton;
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


