#include "ChordsTabControlPanel.h"


ChordsTabControlPanel::ChordsTabControlPanel(ChordsTabControl* parent)
{
	_parent = parent;
	OBJECT_CREATED;
}

ChordsTabControlPanel::~ChordsTabControlPanel()
{
	OBJECT_DELETED;
}


void ChordsTabControlPanel::create(HWND hParentWindow)
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
		throw new RuntimeException("ChordsToolbar::create", "Could not create buttons panel");
	}
}


void ChordsTabControlPanel::show()
{
	assert(_hWindow != NULL);
	ShowWindow(_hWindow, SW_SHOW);
}


void ChordsTabControlPanel::hide()
{
	assert(_hWindow != NULL);
	ShowWindow(_hWindow, SW_HIDE);
}


/**
 * @see MainWindow::forwardMessage
 */
LRESULT CALLBACK ChordsTabControlPanel::forwardMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
	//retrieve *this pointer and then forward message
    ChordsTabControlPanel* panel = (ChordsTabControlPanel*)GetProp(hWindow, "CorrespondingObject");
    if (panel)
    {
        return panel->handleMessage(hWindow, message, wParam, lParam);
	}
    else
    {
        return DefWindowProc(hWindow, message, wParam, lParam);
    }
}

