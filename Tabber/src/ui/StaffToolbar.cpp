#include "StaffToolbar.h"
#include "../ui/ChordsTabControl.h"
#include "../ui/MainWindow.h"


StaffToolbar::StaffToolbar(ChordsTabControl* parent)
	: ChordsTabControlPanel(parent)
{
	OBJECT_CREATED;
}


StaffToolbar::~StaffToolbar()
{
	OBJECT_DELETED;
}


void StaffToolbar::create(HWND hParentWindow)
{
	try
	{
		ChordsTabControlPanel::create(hParentWindow);
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("ChordsToolbar::create", ex);
	}

	//buttons
	addButton(System::getLocaleString(IDS_INSERT_STAFF ),  ID_INSERT_STAFF,   BS_PUSHBUTTON);
	addButton(System::getLocaleString(IDS_INSERT_BAR   ),  ID_INSERT_BAR,     BS_PUSHBUTTON);
	addButton(System::getLocaleString(IDS_INSERT_TUNING),  ID_INSERT_TUNING,  BS_PUSHBUTTON);

	//subclassing (define my own window proc for this control)
	_superClassWindowProc = (WNDPROC)SetWindowLong(_hWindow, GWL_WNDPROC, (long)ChordsTabControlPanel::forwardMessage);

    //store *this pointer in window handle so that I can access class members later
    SetProp(_hWindow, "CorrespondingObject", (void*)this);
}


/**
 * @see MainWindow::handleMessage
 */
LRESULT CALLBACK StaffToolbar::handleMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
	if(message == WM_COMMAND)
	{
		SendMessage(
			_parent->getMainWindow()->getWindowHandle(),
			WM_COMMAND,
			wParam,
			lParam );
		_parent->getMainWindow()->getEditArea()->setFocus();

		return FALSE;
	}
	else
	{
		return CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);
	}
}


/**
 * Enables/Disables all the buttons that correspond to the specified command
 */
void StaffToolbar::setCommandEnabled(int commandId, bool isCommandEnabled)
{
    assert(_hWindow != NULL);

    HWND hButton = GetDlgItem(_hWindow, commandId);
	if(hButton != NULL)
	{
		EnableWindow(hButton, isCommandEnabled);
	}
}


