#include "ChordButtonsPanel.h"
#include "../ui/MainWindow.h"

const char ChordButtonsPanel::WINDOW_CLASS_NAME[] = "ChordButtonsPanel";
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
	static bool windowClassNotRegistered = true;
	
	if(windowClassNotRegistered)
	{
		//I register a window class in order to define my own WndProc
		ZeroMemory(&_windowClass, sizeof(_windowClass));
	    _windowClass.cbSize        = sizeof(WNDCLASSEX);
	    _windowClass.lpfnWndProc   = ChordButtonsPanel::WindowProc;
	    _windowClass.hInstance     = GetModuleHandle(NULL);
	    _windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	    _windowClass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	    _windowClass.lpszClassName = WINDOW_CLASS_NAME;
	
	    if(!RegisterClassEx(&_windowClass))
	    {
	    	throw new RuntimeException("MainWindow::create", "Could not register class");
	    }
	    
    	windowClassNotRegistered = false;
    }

	_hWindow = CreateWindowEx(
		0,
		WINDOW_CLASS_NAME,
		NULL,
		WS_CHILD,
		CW_USEDEFAULT,
  		CW_USEDEFAULT,
    	CW_USEDEFAULT,
     	CW_USEDEFAULT,
		hParentWindow,
  		NULL,
		GetModuleHandle(NULL),
  		NULL );
		
	if(_hWindow == NULL)
	{
		throw new RuntimeException("ChordButtonsPanel::create", "Could not create buttons panel");			
	}
	
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


HWND& ChordButtonsPanel::getWindowHandle()
{
	return _hWindow;
}


/**
 * Win32's message handling function
 */
LRESULT CALLBACK ChordButtonsPanel::WindowProc(
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
	//propagate chord buttons commands
    if(message == WM_COMMAND && LOWORD(wParam) >= IDC_FIRST_CHORD)
    {
		assert(gMainWindow->getWindowHandle());
   		PostMessage(gMainWindow->getWindowHandle(), WM_COMMAND, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWindow, message, wParam, lParam);
	}
    return 0;
}


