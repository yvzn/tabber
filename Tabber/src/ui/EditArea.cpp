#include "EditArea.h" 
#include "../ui/MainWindow.h"


EditArea::EditArea(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;
	_displayFont = NULL;
	OBJECT_CREATED;
}


EditArea::~EditArea()
{
	if(_displayFont != NULL) DeleteObject(_displayFont);

	OBJECT_DELETED;
}


void EditArea::create(HWND hParentWindow)
{
	_hWindow = CreateWindowEx (
		WS_EX_CLIENTEDGE,
		"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
  		ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hParentWindow, (HMENU)IDC_EDIT_AREA, GetModuleHandle(NULL), NULL );

	if(_hWindow == NULL)
	{
		throw new RuntimeException("EditArea::create", "Could not create editing area");
	}
	
	//subclassing (define my own window proc for this control)
	_superClassWindowProc = (WNDPROC)SetWindowLong(_hWindow, GWL_WNDPROC, (long)EditArea::forwardMessage);
    
    //store *this pointer in window handle so that I can access class variables and methods
    SetProp(_hWindow, "CorrespondingObject", (void*)this);

	SetFocus(_hWindow);
	
	LOGFONT font;
 	CopyMemory(&font, &_mainWindow->getApplication()->getSettings()->getEditAreaFont(), sizeof(font));
    //as I store size units in points in the Settings, I need a conversion from point size to logical units here
	font.lfHeight = -MulDiv(font.lfHeight, GetDeviceCaps(GetDC(_mainWindow->getWindowHandle()), LOGPIXELSY), 72);
	setFont(font);
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


void EditArea::doCommand(UINT command)
{
	assert(_hWindow != NULL);
	SendMessage(_hWindow, command, 0, 0);
}


/**
 * @see MainWindow::forwardMessage
 */
LRESULT CALLBACK EditArea::forwardMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
	//retrieve *this pointer and then forward message
    EditArea* editArea = (EditArea*)GetProp(hWindow, "CorrespondingObject");
    if (editArea)
    {
        return editArea->handleMessage(hWindow, message, wParam, lParam);
	}
    else
    {
        return DefWindowProc(hWindow, message, wParam, lParam);
    }
}


/**
 * @see MainWindow::handleMessage
 */
LRESULT CALLBACK EditArea::handleMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
    switch(message)
    {
    	case WM_CUT:
    	case WM_PASTE:
    	case WM_UNDO:
    	{
			_mainWindow->getDocumentInterface()->setDocumentModified(true);
			break;
		}

		// overwrite mode test !!!!!!!!!!
		case WM_CHAR:
		{
			_mainWindow->getDocumentInterface()->setDocumentModified(true);

//*
            if ((wParam != VK_BACK) && (wParam != VK_DELETE))
            {
                WORD wStart,wEnd;
                DWORD dwResult;

                dwResult = SendMessage(_hWindow,EM_GETSEL,0,0L);
                wStart   = LOWORD(dwResult);
                wEnd     = HIWORD(dwResult);

                if (wEnd == wStart)
                {
                   wEnd++;
                   SendMessage(_hWindow,EM_SETSEL,wStart,wEnd);
                }

                SendMessage(_hWindow,EM_REPLACESEL,TRUE, (DWORD)((LPSTR)&wParam ));
                return (FALSE);
            }
//*/
            break;
		}
	}
	
	return CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);
}


void EditArea::wipeContent()
{
	assert(_hWindow != NULL);
	SetWindowText(_hWindow, "");
}


void EditArea::saveContentTo(const char* fileName)
{
	assert(_hWindow != NULL);
	
	DWORD bytesWritten; //useless but required

	HANDLE hFile = CreateFile(
		fileName,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		CREATE_ALWAYS,
		0,
  		NULL );
  		
	if(hFile != INVALID_HANDLE_VALUE)
	{
        DWORD textLength = GetWindowTextLength(_hWindow);
        if(textLength > 0)
        {
            char* writeBuffer = new char[textLength+1];
            GetWindowText(_hWindow, writeBuffer, textLength+1);
        
			if(!WriteFile(hFile, writeBuffer, (DWORD)textLength, &bytesWritten, NULL))
			{
				delete [] writeBuffer;
				throw new RuntimeException("EditArea::saveContentTo", "Could not save document");
			}
			
			delete [] writeBuffer;
		}
	}
	else
	{
		throw new RuntimeException("EditArea::saveContentTo", "Could not open document for saving");
	}
	
	CloseHandle(hFile);

	SetFocus(_hWindow);
}



void EditArea::loadContentFrom(const char* fileName)
{
	assert(_hWindow != NULL);
	
	DWORD bytesRead; //useless but required

	HANDLE hFile = CreateFile(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        DWORD fileSize = GetFileSize(hFile, NULL);
        if(fileSize != 0xFFFFFFFF)
        {
            char* readBuffer = new char[fileSize+1];
            if(ReadFile(hFile, readBuffer, fileSize, &bytesRead, NULL))
            {
                readBuffer[fileSize] = '\0'; // Add null terminator
                SetWindowText(_hWindow, readBuffer);
            }
            else
			{
				delete [] readBuffer;
				throw new RuntimeException("EditArea::loadContentFrom", "Could not load document");
            }
			delete [] readBuffer;
        }
	}
	else
	{
		throw new RuntimeException("EditArea::loadContentFrom", "Could not open document for loading");
	}

	CloseHandle(hFile);

	SetFocus(_hWindow);
}

/*
void EditArea::setFont(const char* fontFamily, int fontSize)
{
	assert(_hWindow != NULL);
    
	LOGFONT font;
	ZeroMemory(&font, sizeof(font));
	
	font.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	lstrcpy(font.lfFaceName, fontFamily);

    //conversion from point size to logical units
	font.lfHeight = -MulDiv(fontSize, GetDeviceCaps(GetDC(_hWindow), LOGPIXELSY), 72);

	setFont(font);	
}
//*/

void EditArea::setFont(const LOGFONT& newValue)
{
	assert(_hWindow != NULL);
	
	if(_displayFont != NULL) DeleteObject(_displayFont);

	_displayFont = CreateFontIndirect(&newValue);
	SendMessage(_hWindow, WM_SETFONT, (WPARAM)_displayFont, MAKELPARAM(TRUE, 0));
}

