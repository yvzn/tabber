#include "DocumentManager.h"
#include "../ui/MainWindow.h"


DocumentManager::DocumentManager(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;
	
	blankDocumentFlags();
	OBJECT_CREATED;
}


DocumentManager::~DocumentManager()
{
	OBJECT_DELETED;
}


void DocumentManager::setDocumentModified(bool newValue)
{
	_isDocumentModified = newValue;
}


void DocumentManager::onNewDocument()
{
	if(continueIfDocumentModified())
	{
		blankDocumentFlags();
		updateMainWindowTitle();
		//edit area: new Document
	}
}


bool DocumentManager::continueIfDocumentModified()
{
	if(_isDocumentModified)
	{
		char message[MAX_PATH];
		lstrcpy(message, "Save changes to '");
		lstrcat(message, _filePath);
		lstrcat(message, "/");
		lstrcat(message, _fileName);
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
      			//onDocumentSave
			}
      		return true;
      	}
	}
	else
	{
		return true;
	}
}


void DocumentManager::blankDocumentFlags()
{
	_isDocumentModified = false;
	_isFileLoaded = false;
	lstrcpy(_fileName, "Untitled");
	lstrcpy(_filePath, "");
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

