#include "EditArea.h"
#include "../ui/MainWindow.h"



// CONSTR AND DESTR ///////////////////////////////////////////////////////////


EditArea::EditArea(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;
	_displayFont = NULL;
	_toolkit = new EditionToolkit(this);
	OBJECT_CREATED;
}


EditArea::~EditArea()
{
	if(_displayFont != NULL) DeleteObject(_displayFont);
	delete _toolkit;
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
    
    //initialise toolkit
	_toolkit->setWindowHandle(_hWindow);
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
	setFocus();
	onSelectionChange();
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

	setFocus();
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

	setFocus();
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
	bool selectionChanged = false;
	bool documentModified = false;

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
			documentModified = true;
			selectionChanged = true;;
			break;
		}
		
    	case WM_PASTE:
    	case WM_UNDO:
    	{
         	documentModified = true;
			selectionChanged = true;
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

			// I do check selection on focus to initialise a few related things
			selectionChanged = true;
      		break;
  		}

		case WM_LBUTTONUP:
  		{
        	selectionChanged = true;
      		break;
  		}

		case WM_KEYUP:
      	{
			if( onKeyUp(wParam) == FALSE )
			{
				return FALSE;
			}
			break;
		}

		case WM_KEYDOWN:
      	{
			if( onKeyDown(wParam) == FALSE )
			{
				return FALSE;
			}
			break;
		}

		case WM_CHAR:
		{
      		// characters keystrokes
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
		      		selectionChanged = true;
 					documentModified = true;
					break;
				}
			}
            break;
		}

		default:
  		{
			break;
		}
	}

	// most subclassed messages still require a call to super class's window proc
	LRESULT result = CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);

	// I make those calls here because CallWindowProc may modify stuff (esp. selection)
	if(documentModified) onDocumentModified();
	if(selectionChanged) onSelectionChange();

	return result;
}



// WIN32's MESSAGES HANDLING (ACTIONS) ////////////////////////////////////////


/**
 * returns TRUE if event must be re-processed after call by Superclass's WindowProc, FALSE if it has been processed internally
 */
LRESULT EditArea::onKeyUp(int virtuakKeyCode)
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
   		{
			onSelectionChange();
			onDocumentModified();
			break;
		}

		case VK_SNAPSHOT: // Print Screen
        {
        	_mainWindow->setCommandEnabled(ID_EDIT_PASTE, false);
			break;
		}

		default:
		{
			break;
		}
	}
	return TRUE;
}


/**
 * returns TRUE if event must be re-processed after call by Superclass's WindowProc, FALSE if it has been processed internally
 */
LRESULT EditArea::onKeyDown(int virtuakKeyCode)
{
	assert(_hWindow != NULL);

	switch(virtuakKeyCode)
  	{
		case VK_DELETE:
   		{
			//handled here because VK_DELETE does not generate those handy WM_CHAR events
			if(_mainWindow->getApplication()->getSettings()->getTypingMode() == SPECIAL)
			{
				DWORD selection = _toolkit->getSelection();
				if( _toolkit->isInsideStaff(selection) )
				{
					if( !_toolkit->doesSelectionEndLine(selection) )
					{
						StaffAction* action = new ForwardRemover();
						apply(action);
						delete action;
					}
					return FALSE;
				}
			}
			break;
		}

		default:
		{
			break;
		}
	}
	return TRUE;
}


void EditArea::onSelectionChange()
{
    assert(_hWindow != NULL);
    DWORD selection = _toolkit->getSelection();
    
	if(_toolkit->isSelectionValid(selection))
	{
		bool isTextSelected = _toolkit->isTextSelected(selection);

	    _mainWindow->setCommandEnabled(ID_EDIT_COPY,   isTextSelected );
	    _mainWindow->setCommandEnabled(ID_EDIT_CUT,    isTextSelected );
	    _mainWindow->setCommandEnabled(ID_EDIT_DELETE, isTextSelected );
    
	    bool isInsideStaff = _toolkit->isInsideStaff(selection);

		_mainWindow->setCommandEnabled(ID_INSERT_BAR,    isInsideStaff );
		_mainWindow->setCommandEnabled(ID_INSERT_TUNING, isInsideStaff );
	}
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



// TYPING MODES ///////////////////////////////////////////////////////////////


/**
 * A character is entered in overwrite typing mode
 */
LRESULT EditArea::onCharOverwriteMode(int virtuakKeyCode)
{
	assert(_hWindow != NULL);

    switch(virtuakKeyCode)
    {
		case VK_BACK:
       	case VK_RETURN:
		case VK_ESCAPE:
		{
      		// special, do nothing
			break;
		}

		default:
		{
            DWORD selection = _toolkit->getSelection();

            if (!_toolkit->isTextSelected(selection))
            {
				//do not overtype at the end of a lines (otherwise it would remove \r and/or \n)
				if(!_toolkit->doesSelectionEndLine(selection))
				{
        			//expand selection by one character so that typing overwrites that extra character
               		_toolkit->setSelection(__startOf(selection), __endOf(selection) + 1);
				}
            }
		}
	}
	
	LRESULT result = CallWindowProc(_superClassWindowProc, _hWindow, WM_CHAR, (WPARAM)virtuakKeyCode, 0);
	onSelectionChange();
	onDocumentModified();
	return result;
}


/**
 * A character is entered in special typing mode
 */
LRESULT EditArea::onCharSpecialMode(int virtuakKeyCode)
{
	DWORD selection = _toolkit->getSelection();

    if( _toolkit->isStaffLine(selection) )
    {
	    switch(virtuakKeyCode)
	    {
			case VK_ESCAPE:
			{
	      		// special, do nothing
				break;
			}

			case VK_BACK:
			{
				unsigned int selEnd = __endOf(selection);
				if( selEnd != _toolkit->getLineStart( _toolkit->getLineIndex( selEnd ) ) )
				{
					StaffAction* action = new BackwardRemover();
					apply(action);
					delete action;
				}
				break;
			}

	       	case VK_RETURN:
			{
				_toolkit->moveToNextLine(selection);
				onSelectionChange();
				onDocumentModified();
				break;
			}

			default:
			{
				int line = _toolkit->getLineIndex( __endOf(selection) );
				StaffAction* action = new SingleNoteDispatcher(virtuakKeyCode, line);
				apply(action);
				delete action;
				break;
			}
		}

		return FALSE;
	}
	else
	{
		LRESULT result = CallWindowProc(_superClassWindowProc, _hWindow, WM_CHAR, (WPARAM)virtuakKeyCode, 0);
		onSelectionChange();
		onDocumentModified();
		return result;
	}
}



// WIN32's MESSAGES HANDLING (INSERTIONS) /////////////////////////////////////


/**
 * Inserts an empty staff at cursor position (or nearby if not possible)
 */
void EditArea::onInsertStaff()
{
	assert(_hWindow != NULL);

	_toolkit->moveToNextLine();

	ApplicationSettings* settings     = _mainWindow->getApplication()->getSettings();

 	TuningIndex    tuningIndex  = settings->getSelectedTuningIndex();
	bool           prefixTuning = tuningIndex > 0; // index 0 means no tuning
	GuitarTuning*  tuning       = prefixTuning ? _mainWindow->getApplication()->getTuningDefinitions()->getTuningAt(tuningIndex-1) : NULL ;

	int    staffHeight  = settings->getChordDepth();
	int    staffWidth   = settings->getStaffWidth();
	int    bufferSize   = 6 /* "\r\n" x 3 */ + staffHeight * (staffWidth + 2 /* "\r\n" */ );
	char*  buffer       = new char[bufferSize + 1]; //+1 for the final '\0';

	//pre-fill -- with '-'s and CR+LFs
	for(int pos=0; pos<bufferSize; ++pos) buffer[pos] = '-'; buffer[bufferSize] = '\0';
	buffer[0] = '\r'; buffer[1] = '\n';
	buffer[2] = '\r'; buffer[3] = '\n';
	buffer[bufferSize-2] = '\r'; buffer[bufferSize-1] = '\n';

	//pointer to the current line (starts after the leading "\r\n"s);
	char* bufferLine = &buffer[4];

	for(int string=0; string<staffHeight; ++string)
	{
		if(prefixTuning)
		{
			_toolkit->copyNoteAtBufferStart(tuning, string, bufferLine);
		}

		//move pointer and adds CR+LF
		bufferLine += staffWidth * sizeof(char);
		bufferLine[0] = '\r';
		bufferLine[1] = '\n';
		bufferLine += 2 * sizeof(char);
	}

	_toolkit->replaceSelection(buffer);
	delete [] buffer;

    onSelectionChange();
	onDocumentModified();
}


/**
 * Inserts current guitar tuning at the beginning of staff line
 */
void EditArea::onInsertTuning()
{
 	TuningIndex tuningIndex  = _mainWindow->getApplication()->getSettings()->getSelectedTuningIndex();

	if(tuningIndex > 0) // index 0 means no tuning
	{
		GuitarTuning* tuning = _mainWindow->getApplication()->getTuningDefinitions()->getTuningAt(tuningIndex-1);

		StaffAction* action = new TuningDispatcher(tuning);
		apply(action);
		delete action;
	}
}


/**
 * Inserts a vertical bar at cursor position
 */
void EditArea::onInsertBar()
{
	StaffAction* action = new BarDispatcher();
	apply(action);
	delete action;
}


/**
 * Inserts a chord at cursor position
 * @param commandId Command identifier of chord to insert
 */
void EditArea::onInsertChord(unsigned int commandId, ArpeggioDispatcher::Direction direction)
{
	if( _toolkit->isInsideStaff() )
	{
		ChordIndex index = GetChordIndex(commandId);
		StaffAction* action;

		if(direction==ArpeggioDispatcher::NONE)
		{
			action = new ChordDispatcher(
				_mainWindow->getApplication()->getChordDefinitions()->getChordGroupAt(index.group)->getChordAt(index.chord) );
		}
		else
		{
			action = new ArpeggioDispatcher(
				_mainWindow->getApplication()->getChordDefinitions()->getChordGroupAt(index.group)->getChordAt(index.chord),
				direction );
		}

		apply(action);
		delete action;
	}

	setFocus();
}



// UTILITIES //////////////////////////////////////////////////////////////////


void EditArea::apply(StaffAction* action)
{
	assert(_hWindow != NULL && _toolkit->isInsideStaff());

	DWORD selection = _toolkit->getSelection();
	_toolkit->saveCursorPosition(selection);

	unsigned int column = _toolkit->getColumnIndex(__endOf(selection));
	unsigned int firstLine = _toolkit->getStaffFirstLineIndex(selection);
	unsigned int lastLine = _toolkit->getStaffLastLineIndex(selection);

	for(unsigned int line=firstLine; line<=lastLine; ++line)
	{
		action->applyAt(line, column, _toolkit);
	}

	if(_mainWindow->getApplication()->getSettings()->isChordModeEnabled(ADD_NAME) && firstLine>0)
	{
		//fill header line if needed
		unsigned int headerLine = firstLine - 1;
  		unsigned int headerLength = _toolkit->getLineLength( _toolkit->getLineStart(headerLine) );
		if( headerLength < column )
		{
			_toolkit->copyAndFillAtLineCol("", headerLine, headerLength, column - headerLength, ' ');
		}

		//add header (chord names, spaces, ...) at column
		action->applyAt(headerLine, column, _toolkit, true);
	}

	_toolkit->restoreCursorPosition(action->getActionOffset());
	onDocumentModified();
}

