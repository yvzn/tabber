#include "DocumentManager.h"
#include "../ui/MainWindow.h"


DocumentManager::DocumentManager(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;

	blankDocumentFlags();
	
	ZeroMemory(&_fileDialogOptions, sizeof(_fileDialogOptions));
    _fileDialogOptions.lStructSize = sizeof(_fileDialogOptions);
    _fileDialogOptions.lpstrFilter = "Tablatures (*.tab;*.crd)\0*.tab;*.crd\0Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    _fileDialogOptions.lpstrFile = _filePathAndName;
    _fileDialogOptions.nMaxFile = MAX_PATH;
    _fileDialogOptions.lpstrDefExt = "tab";   
	
	OBJECT_CREATED;
}


DocumentManager::~DocumentManager()
{
	OBJECT_DELETED;
}


void DocumentManager::setDocumentModified(bool newValue)
{
	if(_isDocumentModified != newValue)
	{
		_isDocumentModified = newValue;
		updateMainWindowTitle();
	}
}


void DocumentManager::blankDocumentFlags()
{
	lstrcpy(_filePathAndName, "Untitled");
	_fileName = _filePathAndName;
	_isFileLoaded = false;
	_isDocumentModified = false;
}


void DocumentManager::updateMainWindowTitle()
{
	char buffer[MAX_PATH];

	lstrcpy(buffer, _fileName);
	if(_isDocumentModified) lstrcat(buffer, "*");
	lstrcat(buffer, " - ");
	lstrcat(buffer, MainWindow::APPLICATION_NAME);
	
	_mainWindow->setWindowTitle(buffer);
}


void DocumentManager::onNewDocument()
{
	if(continueIfDocumentModified())
	{
		blankDocumentFlags();
		updateMainWindowTitle();
		_mainWindow->getEditArea()->wipeContent();
	}
}


bool DocumentManager::continueIfDocumentModified()
{
	if(_isDocumentModified)
	{
		char message[MAX_PATH];
		lstrcpy(message, "Save changes to '");
		lstrcat(message, _filePathAndName);
		lstrcat(message, "' ?");
		
    	int answer = MessageBox (
     		_mainWindow->getWindowHandle(),
       		message,
         	MainWindow::APPLICATION_NAME,
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
bool DocumentManager::onDocumentSave()
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
bool DocumentManager::onDocumentSaveAs()
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
			NotifyMessage::publicError("Could not save file !\nMaybe file has \'read only\' attribute ?");
			delete ex;
			return false;
		}
    }
	else
	{
		return false;
	}
}


void DocumentManager::onDocumentOpen()
{
	if(continueIfDocumentModified())
	{
	    _fileDialogOptions.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
	    _fileDialogOptions.hwndOwner = _mainWindow->getWindowHandle();
		if(GetOpenFileName(&_fileDialogOptions))
		{
			try
			{
				_mainWindow->getEditArea()->loadContentFrom(_filePathAndName);
		        _isFileLoaded = true;
				_isDocumentModified = false;			
		        updateFileName();
				updateMainWindowTitle();			
			}
			catch(RuntimeException* ex)
			{
				NotifyMessage::publicError("Could not open file !");
				delete ex;
			}
		}
	
	}
}


/**
 * Updates _fileName pointer (a substring of _filePathAndName)
 */
void DocumentManager::updateFileName()
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


