/*
 * Tabber - Guitar Tablature Editor, freeware and open-source
 * Copyright (C) 2004  Yvan Razafindramanana
 *
 * Additionnal Contributors:
 * <place your name here :)>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "DocumentInterface.h"
#include "../../ui/MainWindow.h"

bool DocumentInterface::_isDialogOpened = false;


// CONSTR AND DESTR ///////////////////////////////////////////////////////////


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

	_findWhat = new char[80];
	_findWhat[0] = '\0';
	_replaceWith = new char[80];
	_replaceWith[0] = '\0';

	ZeroMemory(&_findReplaceOptions, sizeof(_findReplaceOptions));
	_findReplaceOptions.lStructSize = sizeof(_findReplaceOptions);
	_findReplaceOptions.lpstrFindWhat = _findWhat;
	_findReplaceOptions.wFindWhatLen = 80;
	_findReplaceOptions.lpstrReplaceWith = _replaceWith;
	_findReplaceOptions.wReplaceWithLen = 80;
	_findReplaceOptions.Flags = FR_HIDEUPDOWN | FR_HIDEWHOLEWORD | FR_ENABLEHOOK;
	_findReplaceOptions.lpfnHook = DocumentInterface::handleFindReplaceDialogMessage;

	_hFindReplaceDialog = NULL;

	OBJECT_CREATED;
}


DocumentInterface::~DocumentInterface()
{
	delete [] _findWhat;
	delete [] _replaceWith;

	OBJECT_DELETED;
}



// DOCUMENT IO ////////////////////////////////////////////////////////////////


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
	lstrcpy(_filePathAndName, System::getLocaleString(IDS_UNTITLED));
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
		updateMainWindowTitle();
		_mainWindow->getEditArea()->wipeContent();
	}
}


bool DocumentInterface::continueIfDocumentModified()
{
	if(_isDocumentModified)
	{
		char* message = new char[20 + MAX_PATH];
		wsprintf(message, System::getLocaleString(IDS_SAVE_CHANGES), _filePathAndName);
		
    	int answer = MessageBox (
     		_mainWindow->getWindowHandle(),
       		message,
			System::getLocaleString(IDS_CONFIRMATION),
          	MB_ICONQUESTION | MB_YESNOCANCEL );

		delete [] message;

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
			NotifyMessage::publicError(
				_mainWindow->getWindowHandle(),
				System::getLocaleString(IDERR_SAVE_DOCUMENT),
				_filePathAndName );

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
			NotifyMessage::publicError (
				_mainWindow->getWindowHandle(),
				System::getLocaleString(IDERR_SAVE_DOCUMENT),
				_filePathAndName );
			delete ex;
			return false;
		}
    }
	else
	{
		return false;
	}
}

/**
 * @returns true if document was indeed opened -- false if user canceled
 */
bool DocumentInterface::onDocumentOpen()
{
	if(continueIfDocumentModified())
	{
	    _fileDialogOptions.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	    _fileDialogOptions.hwndOwner = _mainWindow->getWindowHandle();
		if(GetOpenFileName(&_fileDialogOptions))
		{
        	return loadSpecifiedDocument();
		}
	}
	return false;
}


/**
 * @returns true if document was indeed opened -- false if user canceled
 */
bool DocumentInterface::onDocumentOpen(const char* fileName)
{
	if(continueIfDocumentModified())
	{
     	lstrcpy(_filePathAndName, fileName);
		return loadSpecifiedDocument();
	}
	return false;
}


/**
 * Loads the document specified in _filePathAndName member. An error message is displayed if operation fails.
 * @returns true if document was indeed opened -- false if an error occured
 */
bool DocumentInterface::loadSpecifiedDocument()
{
	assert(lstrlen(_filePathAndName) > 0);
	
	try
	{
		_mainWindow->getEditArea()->loadContentFrom(_filePathAndName);
	    _isFileLoaded = true;
		setDocumentModified(false);
	    updateFileName();
		updateMainWindowTitle();
		return true;
	}
	catch(RuntimeException* ex)
	{
		NotifyMessage::publicError(
			_mainWindow->getWindowHandle(),
			System::getLocaleString(IDERR_OPEN_DOCUMENT),
			_filePathAndName );

		delete ex;
		return false;
	}
	return true;
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
	
	//move forward to remove the leading backslash
	if(_fileName[0] == '\\')
	{
		_fileName += sizeof(char);
	}	
}



// FIND AND REPLACE ///////////////////////////////////////////////////////////


void DocumentInterface::onFind()
{
	if(!_isDialogOpened)
	{
		_isDialogOpened = true;
		_findReplaceOptions.hwndOwner = _mainWindow->getWindowHandle();
		_hFindReplaceDialog = FindText(&_findReplaceOptions);
	}
}


void DocumentInterface::onFindNext()
{
	if(lstrlen(_findReplaceOptions.lpstrFindWhat) > 0)
	{
		_findReplaceOptions.Flags |= FR_FINDNEXT;
		_mainWindow->getEditArea()->onFindReplace(&_findReplaceOptions);
	}
	else
	{
		onFind();
	}
}


void DocumentInterface::onReplace()
{
	if(!_isDialogOpened)
	{
		_isDialogOpened = true;
		_findReplaceOptions.hwndOwner = _mainWindow->getWindowHandle();
		_hFindReplaceDialog = ReplaceText(&_findReplaceOptions);
	}
}


UINT APIENTRY DocumentInterface::handleFindReplaceDialogMessage(
	HWND hDialog,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	switch (message)
	{
		case WM_INITDIALOG: return TRUE;
		case WM_GETDLGCODE: return DLGC_WANTALLKEYS;
		case WM_COMMAND   : if(LOWORD(wParam) == IDCANCEL)
		case WM_CLOSE     : _isDialogOpened = false;
	}

	return FALSE;
}
