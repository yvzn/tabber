#include "StaffToolbar.h"


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

	return CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);
}



