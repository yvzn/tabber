#include "EditArea.h"
#include "../ui/MainWindow.h"



// CONSTR AND DESTR ///////////////////////////////////////////////////////////


EditArea::EditArea(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;
	_displayFont = NULL;
	_heuristics = new EditionHeuristics(this);
	OBJECT_CREATED;
}


EditArea::~EditArea()
{
	if(_displayFont != NULL) DeleteObject(_displayFont);
	delete _heuristics;
	OBJECT_DELETED;
}



// WINDOW CREATION ////////////////////////////////////////////////////////////


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
    
    //store *this pointer in window handle so that I can access class members later
    SetProp(_hWindow, "CorrespondingObject", (void*)this);
    
    //initialise heuristics
	_heuristics->setWindowHandle(_hWindow);
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



// WINDOW MANAGEMENT //////////////////////////////////////////////////////////


void EditArea::setFont(const LOGFONT& newValue)
{
	assert(_hWindow != NULL);

	if(_displayFont != NULL) DeleteObject(_displayFont);

	_displayFont = CreateFontIndirect(&newValue);
	SendMessage(_hWindow, WM_SETFONT, (WPARAM)_displayFont, MAKELPARAM(TRUE, 0));
}


void EditArea::setFocus()
{
	assert(_hWindow != NULL);
	SetFocus(_hWindow);
}


// CONTENT MANAGEMENT /////////////////////////////////////////////////////////


void EditArea::wipeContent()
{
	assert(_hWindow != NULL);
	SetWindowText(_hWindow, "");
	SetFocus(_hWindow);
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



// WIN32's MESSAGES HANDLING (DISPATCHERS) ////////////////////////////////////


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
	//retrieve instance pointer and then forward message
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
        case WM_COPY:
        {
			_mainWindow->setCommandEnabled(ID_EDIT_PASTE, true);
			break;
		}
		
    	case WM_CUT:
		{
			_mainWindow->setCommandEnabled(ID_EDIT_PASTE, true);
			onDocumentModified();
			onSelectionChange();
			break;
		}
		
    	case WM_PASTE:
    	case WM_UNDO:
    	{
         	onDocumentModified();
			break;
		}

		case WM_SETFOCUS:
  		{
  		    // setfocus is quite a good moment to perform some general UI updates
  		    _mainWindow->setCommandEnabled(
        		ID_EDIT_PASTE,
      		 	IsClipboardFormatAvailable(CF_TEXT) != 0 );
  		    _mainWindow->setCommandEnabled(
        		ID_EDIT_UNDO,
          		SendMessage(hWindow, EM_CANUNDO, 0, 0) != 0 );

			// I do check selection on focus to initialise a few things related on selection
			onSelectionChange();
      		break;
  		}

		case WM_LBUTTONUP:
  		{
        	onSelectionChange();
      		break;
  		}

		case WM_KEYUP:
      	{
			// special keystrokes
			onKeyUp(wParam);
			break;
		}
		
		case WM_CHAR:
		{
      		// characters keystrokes
      		
 			onDocumentModified();
 			switch(_mainWindow->getApplication()->getSettings()->getTypingMode())
 			{
				case OVERWRITE:
        		{
					return onCharOverwriteMode(wParam);
				}

				case SPECIAL:
        		{
              		return onCharSpecialMode(wParam);
           		}
           		
				default:
        		{
					break;
				}
			}
            break;
		}
	}

	// most subclassed messages still require a call to super class's window proc
	return CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);
}



// WIN32's MESSAGES HANDLING (ACTIONS) ////////////////////////////////////////


void EditArea::onKeyUp(int virtuakKeyCode)
{
	switch(virtuakKeyCode)
  	{
		case VK_PRIOR:
		case VK_NEXT:
		case VK_END:
		case VK_HOME:
		case VK_LEFT:
		case VK_UP:
		case VK_RIGHT:
		case VK_DOWN:
   		{
			onSelectionChange();
			break;
		}

		case VK_DELETE:
   		case VK_BACK:
   		{
			onDocumentModified();
			onSelectionChange();
			break;
		}

		case VK_SNAPSHOT: // Print Screen
        {
        	_mainWindow->setCommandEnabled(ID_EDIT_PASTE, false);
			break;
		}
	}
}


void EditArea::onSelectionChange()
{
    assert(_hWindow != NULL);
    
    DWORD selection = SendMessage(_hWindow, EM_GETSEL, 0, 0L);
    
	bool isTextSelected = _heuristics->isTextSelected(selection);

    _mainWindow->setCommandEnabled(ID_EDIT_COPY,   isTextSelected );
    _mainWindow->setCommandEnabled(ID_EDIT_CUT,    isTextSelected );
    _mainWindow->setCommandEnabled(ID_EDIT_DELETE, isTextSelected );
    
    bool isInsideStaff = _heuristics->isInsideStaff(selection);

	_mainWindow->setCommandEnabled(ID_INSERT_BAR,    isInsideStaff );
	_mainWindow->setCommandEnabled(ID_INSERT_TUNING, isInsideStaff );
}


void EditArea::onDocumentModified()
{
	_mainWindow->getDocumentInterface()->setDocumentModified(true);
}


void EditArea::onSelectAll()
{
    assert(_hWindow != NULL);
    SendMessage(_hWindow, EM_SETSEL, 0, (LPARAM)-1);
    onSelectionChange();
}


void EditArea::onDelete()
{
    assert(_hWindow != NULL);
    SendMessage(_hWindow, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)"");
    onSelectionChange();
	onDocumentModified();
}


/**
 * A character is entered in overwrite typing mode
 */
LRESULT EditArea::onCharOverwriteMode(int virtuakKeyCode)
{
	assert(_hWindow != NULL);

    switch(virtuakKeyCode)
    {
		case VK_DELETE:
		case VK_BACK:
       	case VK_RETURN:
		case VK_ESCAPE:
		{
      		// special, do nothing
			break;
		}

		default:
		{
            DWORD selection = SendMessage(_hWindow, EM_GETSEL, 0, 0);

            if (!_heuristics->isTextSelected(selection))
            {
				//do not overtype at the end of a lines (otherwise it would remove \r and/or \n)
				if(!_heuristics->doesSelectionEndLine(selection))
				{
        			//expand selection by one character so that typing overwrites that extra character
		            WORD  selStart  = LOWORD(selection);
		            WORD  selEnd    = HIWORD(selection);
		            
               		selEnd++;
               		SendMessage(_hWindow, EM_SETSEL, selStart, selEnd);
				}
            }

            //SendMessage(_hWindow, EM_REPLACESEL, TRUE, (DWORD)((LPSTR)&virtuakKeyCode ));
            //return FALSE;
		}
	}
	
	return CallWindowProc(_superClassWindowProc, _hWindow, WM_CHAR, (WPARAM)virtuakKeyCode, 0);
}


/**
 * A character is entered in special typing mode
 */
LRESULT EditArea::onCharSpecialMode(int virtuakKeyCode)
{
    if(_heuristics->isInsideStaff())
    {
		return FALSE;
	}
	else
	{
		return CallWindowProc(_superClassWindowProc, _hWindow, WM_CHAR, (WPARAM)virtuakKeyCode, 0);
	}
}



void EditArea::onInsertStaff()
{
	assert(_hWindow != NULL);

	_heuristics->moveToNextLine();

	ApplicationSettings* settings     = _mainWindow->getApplication()->getSettings();

	//get guitar tuning
 	TuningIndex    tuningIndex  = settings->getSelectedTuningIndex();
	bool           prefixTuning = tuningIndex > 0; // index 0 means no tuning
	GuitarTuning*  tuning       = prefixTuning ? _mainWindow->getApplication()->getTuningDefinitions()->getTuningAt(tuningIndex-1) : NULL ;

	//create / fill line buffer with '-'
	int    lineSize     = settings->getStaffWidth();
	char*  lineBuffer   = new char[lineSize + 1]; // +1 for the trailing '\0'
	for(int pos=0; pos<lineSize; ++pos) lineBuffer[pos] = '-'; lineBuffer[lineSize] = '\0';

	//create global buffer
	int    staffHeight  = settings->getChordDepth();
	char*  fullBuffer   = new char[6 /* "\r\n" x 3 */ + staffHeight * (lineSize + 2 /* "\r\n" */ ) + 1 /* "\0" */];
	lstrcpy(fullBuffer, "\r\n\r\n");

	//push lines
	for(int string=0; string<staffHeight; ++string)
	{
		if(prefixTuning)
		{
   			for(int prefix=0; prefix<tuning->getWidth(); ++prefix) lineBuffer[prefix]=' '; // blanks the beginning

			if(string<tuning->getNoteCount()) // adds tuning note, if defined
			{
				const char* note = tuning->getNote(tuning->getNoteCount() - string - 1); // notes order is reversed :/
				lstrcpy(lineBuffer, note);
				lineBuffer[lstrlen(note)] = ' '; // removes the '\0' at the end of note
				lineBuffer[tuning->getWidth()] = '|';
			}
		}

		lstrcat(fullBuffer, lineBuffer);
		lstrcat(fullBuffer, "\r\n");
	}

	lstrcat(fullBuffer, "\r\n");
	SendMessage(_hWindow, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)fullBuffer);

	delete [] fullBuffer;
	delete [] lineBuffer;

    onSelectionChange();
	onDocumentModified();
}
