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
    
    int buttonCount = 9;
    
   	TBBUTTON* buttons = new TBBUTTON[buttonCount];
    ZeroMemory(buttons, sizeof(buttons));
	for(int index=0; index<buttonCount; ++index)
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

    buttons[4].iBitmap = STD_UNDO;
    buttons[4].idCommand = ID_EDIT_UNDO;

    buttons[5].fsStyle = TBSTYLE_SEP;

    buttons[6].iBitmap = STD_CUT;
    buttons[6].idCommand = ID_EDIT_CUT;

    buttons[7].iBitmap = STD_COPY;
    buttons[7].idCommand = ID_EDIT_COPY;

    buttons[8].iBitmap = STD_PASTE;
    buttons[8].idCommand = ID_EDIT_PASTE;

    SendMessage(_hWindow, TB_ADDBUTTONS, buttonCount, (LPARAM)buttons);
    
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
