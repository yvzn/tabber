#include "MainToolbar.h"


MainToolbar::MainToolbar()
{
	OBJECT_CREATED;
}


MainToolbar::~MainToolbar()
{
	OBJECT_DELETED;
}


void MainToolbar::create(HWND hParentWindow)
{
	_hWindow = CreateWindowEx (
		0,
		TOOLBARCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT,
  		CW_USEDEFAULT,
    	CW_USEDEFAULT,
     	CW_USEDEFAULT,
		hParentWindow,
		(HMENU)IDC_MAIN_TOOLBAR,
		GetModuleHandle(NULL),
		NULL );

	if(_hWindow == NULL)
	{
		throw new RuntimeException("MainToolbar::create", System::getLocaleString(IDERR_CREATE_WINDOW));
	}
	
	// Send the TB_BUTTONSTRUCTSIZE message, which is required for backward compatibility.
 	SendMessage(_hWindow, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    TBADDBITMAP addBitmapParams;
	addBitmapParams.hInst = HINST_COMMCTRL;
    addBitmapParams.nID = IDB_STD_SMALL_COLOR;
    SendMessage(_hWindow, TB_ADDBITMAP, 0, (LPARAM)&addBitmapParams); 
    
    _buttonCount = 14;
    
   	TBBUTTON* buttons = new TBBUTTON[_buttonCount];
    ZeroMemory(buttons, sizeof(buttons));
	for(int index=0; index<_buttonCount; ++index)
	{
	    buttons[index].fsState = TBSTATE_ENABLED;
	    buttons[index].fsStyle = TBSTYLE_BUTTON;	
	}

    buttons[0].iBitmap = STD_FILENEW;
    buttons[0].idCommand = ID_FILE_NEW;

    buttons[1].iBitmap = STD_FILEOPEN;
    buttons[1].idCommand = ID_FILE_OPEN;

    buttons[2].iBitmap = STD_FILESAVE;
    buttons[2].idCommand = ID_FILE_SAVE;

    buttons[3].fsStyle = TBSTYLE_SEP;

    buttons[4].iBitmap = STD_PRINT;
    buttons[4].idCommand = ID_FILE_PRINT;

    buttons[5].fsStyle = TBSTYLE_SEP;

    buttons[6].iBitmap = STD_CUT;
    buttons[6].idCommand = ID_EDIT_CUT;

    buttons[7].iBitmap = STD_COPY;
    buttons[7].idCommand = ID_EDIT_COPY;

    buttons[8].iBitmap = STD_PASTE;
    buttons[8].idCommand = ID_EDIT_PASTE;

    buttons[9].fsStyle = TBSTYLE_SEP;

    buttons[10].iBitmap = STD_UNDO;
    buttons[10].idCommand = ID_EDIT_UNDO;

    buttons[11].fsStyle = TBSTYLE_SEP;

    buttons[12].iBitmap = STD_FIND;
    buttons[12].idCommand = ID_EDIT_FIND;

    buttons[13].iBitmap = STD_REPLACE;
    buttons[13].idCommand = ID_EDIT_REPLACE;


    SendMessage(_hWindow, TB_ADDBUTTONS, _buttonCount, (LPARAM)buttons);
    
    delete [] buttons;
}


void MainToolbar::resize()
{
	assert(_hWindow != NULL);
	//Toolbar can resize itself automatically inside parent window
	SendMessage(_hWindow, TB_AUTOSIZE, 0, 0);
}


RECT MainToolbar::getSize()
{
	assert(_hWindow != NULL);
	
	RECT windowRect;
	GetWindowRect(_hWindow, &windowRect);
	
	return windowRect;
}


/**
 * Enables/Disables all the buttons that correspond to the specified command
 */
void MainToolbar::setCommandEnabled(int commandId, bool isCommandEnabled)
{
    assert(_hWindow != NULL);

    LPARAM enableFlag = (LPARAM)MAKELONG(isCommandEnabled ? TBSTATE_ENABLED	: TBSTATE_INDETERMINATE, 0);
    SendMessage(_hWindow, TB_SETSTATE, (WPARAM)commandId, enableFlag);   
}

