#include "DocumentInterface.h"
#include "../ui/MainWindow.h"


DocumentInterface::DocumentInterface(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;

	blankDocumentFlags();
	_isDocumentModified = false;

	ZeroMemory(&_fileDialogOptions, sizeof(_fileDialogOptions));
    _fileDialogOptions.lStructSize = sizeof(_fileDialogOptions);
    _fileDialogOptions.lpstrFilter = "Tablatures (*.tab;*.crd)\0*.tab;*.crd\0Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    _fileDialogOptions.lpstrFile = _filePathAndName;
    _fileDialogOptions.nMaxFile = MAX_PATH;
    _fileDialogOptions.lpstrDefExt = "tab";   
	
	OBJECT_CREATED;
}


DocumentInterface::~DocumentInterface()
{
	OBJECT_DELETED;
}


void DocumentInterface::setDocumentModified(bool newValue)
{
	if(_isDocumentModified != newValue)
	{
		_isDocumentModified = newValue;
		updateMainWindowTitle();

	    //placed here for performance
	    _mainWindow->setCommandEnabled(ID_EDIT_UNDO, newValue);
	}
}


void DocumentInterface::blankDocumentFlags()
{
	lstrcpy(_filePathAndName, "Untitled");
	_fileName = _filePathAndName;
	_isFileLoaded = false;
	//_isDocumentModified = false; this flag must be handled separately using
 	// setDocumentModified, because it involves more complex operations
}


void DocumentInterface::updateMainWindowTitle()
{
	char buffer[MAX_PATH];

	lstrcpy(buffer, _fileName);
	if(_isDocumentModified) lstrcat(buffer, "*");
	lstrcat(buffer, " - ");
	lstrcat(buffer, MainWindow::APPLICATION_NAME);
	
	_mainWindow->setWindowTitle(buffer);
}


void DocumentInterface::onNewDocument()
{
	if(continueIfDocumentModified())
	{
		blankDocumentFlags();
		setDocumentModified(false);
		_mainWindow->getEditArea()->wipeContent();
	}
}


bool DocumentInterface::continueIfDocumentModified()
{
	if(_isDocumentModified)
	{
		char message[20 + MAX_PATH];
		lstrcpy(message, "Save changes to '");
		lstrcat(message, _filePathAndName);
		lstrcat(message, "' ?");

    	int answer = MessageBox (
     		_mainWindow->getWindowHandle(),
       		message,
         	"Confirmation",
          	MB_ICONQUESTION | MB_YESNOCANCEL );
          	
      	if(answer == IDCANCEL)
      	{
      		return false;
      	}
      	else
      	{
      		if(answer == IDYES)
      		{
      			return onDocumentSave();
			}
      		return true;
      	}
	}
	else
	{
		return true;
	}
}


/**
 * @returns true if document was indeed saved -- false if user canceled saving
 */
bool DocumentInterface::onDocumentSave()
{
	assert(
 		(_isFileLoaded && lstrlen(_filePathAndName) > 0)
 		|| !_isFileLoaded );
	
	if(_isFileLoaded)
	{
		try
  		{
    		_mainWindow->getEditArea()->saveContentTo(_filePathAndName);
			_isDocumentModified = false;
			updateMainWindowTitle();
			return true;		
		}
		catch(RuntimeException* ex)
		{
			NotifyMessage::publicError("Could not save file !\nMaybe file has \'read only\' attribute ?");
			delete ex;
			return false;
		}
	}
	else
	{
		return onDocumentSaveAs();
	}
}


/**
 * @returns true if document was indeed saved -- false if user canceled saving
 */
bool DocumentInterface::onDocumentSaveAs()
{
    _fileDialogOptions.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    _fileDialogOptions.hwndOwner = _mainWindow->getWindowHandle();
    if(GetSaveFileName(&_fileDialogOptions))
    {
    	try
    	{
    		_mainWindow->getEditArea()->saveContentTo(_filePathAndName);
	        _isFileLoaded = true;
			_isDocumentModified = false;
	        updateFileName();
			updateMainWindowTitle();
			return true;
		}
		catch(RuntimeException* ex)
		{
			NotifyMessage::publicError("Could not save file !\nMaybe it has \'read only\' attribute set ?");
			delete ex;
			return false;
		}
    }
	else
	{
		return false;
	}
}


void DocumentInterface::onDocumentOpen()
{
	if(continueIfDocumentModified())
	{
	    _fileDialogOptions.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	    _fileDialogOptions.hwndOwner = _mainWindow->getWindowHandle();
		if(GetOpenFileName(&_fileDialogOptions))
		{
        	loadSpecifiedDocument();
		}
	}
}


void DocumentInterface::onDocumentOpen(const char* fileName)
{
	if(continueIfDocumentModified())
	{
     	lstrcpy(_filePathAndName, fileName);
		loadSpecifiedDocument();
	}
}


/**
 * Loads the document specified in _filePathAndName member. An error message is displayed if operation fails.
 */
void DocumentInterface::loadSpecifiedDocument()
{
	assert(lstrlen(_filePathAndName) > 0);
	
	try
	{
		_mainWindow->getEditArea()->loadContentFrom(_filePathAndName);
	    _isFileLoaded = true;
		setDocumentModified(false);
	    updateFileName();
		updateMainWindowTitle();
	}
	catch(RuntimeException* ex)
	{
		NotifyMessage::publicError("Could not open file !\nMaybe it is used by another application ?");
		delete ex;
	}
}

/**
 * Updates _fileName pointer (a substring of _filePathAndName)
 */
void DocumentInterface::updateFileName()
{
	assert(lstrlen(_filePathAndName) > 0);
	
	// ugly pointer manipulation :(
	// _fileName is moved to the last character of _filePathAndName and then
	// moved backwards until a backslash \\ is found
	_fileName = &_filePathAndName[lstrlen(_filePathAndName)-1];
	
	while(_fileName[0] != '\\' && _fileName >= _filePathAndName)
	{
		_fileName -= sizeof(char);
	}
	
	//move forward to remove the leading backslash \\
	if(_fileName[0] == '\\')
	{
		_fileName += sizeof(char);
	}	
}


