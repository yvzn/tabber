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
		0, 0, 0, 0,
		hParentWindow, (HMENU)IDC_MAIN_TOOLBAR, GetModuleHandle(NULL), NULL );

	if(_hWindow == NULL)
	{
		throw new RuntimeException("MainToolbar::create", "Could not create main toolbar");	
	}
	
	// Send the TB_BUTTONSTRUCTSIZE message, which is required for backward compatibility.
 	SendMessage(_hWindow, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    TBADDBITMAP addBitmapParams;
	addBitmapParams.hInst = HINST_COMMCTRL;
    addBitmapParams.nID = IDB_STD_SMALL_COLOR;
    SendMessage(_hWindow, TB_ADDBITMAP, 0, (LPARAM)&addBitmapParams); 
    
   	TBBUTTON buttons[3];
    ZeroMemory(buttons, sizeof(buttons));

    buttons[0].iBitmap = STD_FILENEW;
    buttons[0].fsState = TBSTATE_ENABLED;
    buttons[0].fsStyle = TBSTYLE_BUTTON;
    buttons[0].idCommand = ID_FILE_NEW;

    buttons[1].iBitmap = STD_FILEOPEN;
    buttons[1].fsState = TBSTATE_ENABLED;
    buttons[1].fsStyle = TBSTYLE_BUTTON;
    buttons[1].idCommand = ID_FILE_OPEN;

    buttons[2].iBitmap = STD_FILESAVE;
    buttons[2].fsState = TBSTATE_ENABLED;
    buttons[2].fsStyle = TBSTYLE_BUTTON;
    buttons[2].idCommand = ID_FILE_SAVE;

    SendMessage(_hWindow, TB_ADDBUTTONS, sizeof(buttons)/sizeof(TBBUTTON), (LPARAM)&buttons);
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
