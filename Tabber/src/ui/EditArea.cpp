#include "EditArea.h" 
#include "../ui/MainWindow.h"


EditArea::EditArea(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;
	OBJECT_CREATED;
}


EditArea::~EditArea()
{
	OBJECT_DELETED;
}


void EditArea::create(HWND hParentWindow)
{
	_hWindow = CreateWindowEx (
		WS_EX_CLIENTEDGE,
		"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hParentWindow, (HMENU)IDC_EDIT_AREA, GetModuleHandle(NULL), NULL );

	if(_hWindow == NULL)
	{
		throw new RuntimeException("EditArea::create", "Could not create editing area");
	}
	
	SetFocus(_hWindow);
}


void EditArea::resize(const RECT& newSize)
{
	assert(_hWindow != NULL);
	SetWindowPos(
 		_hWindow,
   		0,
		newSize.left,
  		newSize.top,
    	newSize.right - newSize.left,
     	newSize.bottom - newSize.top, 
		0 );
}

