#include "ChordsTabControlPanel.h"


ChordsTabControlPanel::ChordsTabControlPanel(ChordsTabControl* parent)
{
	_parent = parent;
	_xNextButtonPosition = 0;
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
		throw new RuntimeException("ChordsToolbar::create", System::getLocaleString(IDERR_CREATE_WINDOW));
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


HWND ChordsTabControlPanel::addButton(const char* buttonLabel, int buttonId, DWORD buttonStyle)
{
	assert(_hWindow != NULL);

	HWND hButton = CreateWindowEx(
		0,
		"BUTTON",
		buttonLabel,
		WS_CHILD | WS_VISIBLE | buttonStyle,
		_xNextButtonPosition,
		getButtonVerticalPadding(),
		getButtonWidth(),
		getButtonHeight(),
		_hWindow,
		(HMENU)buttonId,
   		GetModuleHandle(NULL),
  		NULL );

	if(hButton == NULL)
	{
		throw new RuntimeException("ChordsTabControlPanel::addButton", System::getLocaleString(IDERR_CREATE_WINDOW));
	}


	System::applyGUIDefaultFont(hButton);

	_xNextButtonPosition += getButtonWidth() + getButtonHorizontalPadding();

	return hButton;
}
