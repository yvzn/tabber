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

#include "MainWindow.h"

const char MainWindow::WINDOW_CLASS_NAME[] = "MainWindow";
const char MainWindow::APPLICATION_NAME[] = "Tabber";



// CONSTR AND DESTR ///////////////////////////////////////////////////////////


MainWindow::MainWindow(Application* application)
{
	InitCommonControls();

	_application      = application;

	_toolbar          = new MainToolbar();
	_status           = new StatusBar();
	_chordsTabControl = new ChordsTabControl(this);
	_editArea         = new EditArea(this);
	_staff            = new StaffToolbar(_chordsTabControl);

	_documentInterface = new DocumentInterface(this);
	_settingsInterface = new SettingsInterface(this);
	_printerInterface  = new PrinterInterface(this);

	OBJECT_CREATED;
}


MainWindow::~MainWindow()
{
	delete _printerInterface;
	delete _settingsInterface;
	delete _documentInterface;

	delete _staff;
	delete _editArea;
	delete _chordsTabControl;
	delete _toolbar;
	delete _status;

	OBJECT_DELETED;
}



// WINDOW CREATION ////////////////////////////////////////////////////////////


/**
 * @throws RuntimeException if an initialisation step screwed up
 */
void MainWindow::create(HINSTANCE hApplicationInstance)
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize        = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc   = MainWindow::forwardMessage;
	wndClass.hInstance     = hApplicationInstance;
	wndClass.hIcon         = LoadIcon(hApplicationInstance, MAKEINTRESOURCE(IDI_ICON_LARGE));
	wndClass.hIconSm       = LoadIcon(hApplicationInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));
	wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);
	wndClass.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
	wndClass.lpszClassName = MainWindow::WINDOW_CLASS_NAME;

	if(!RegisterClassEx(&wndClass))
	{
		throw new RuntimeException("MainWindow::create", System::getLocaleString(IDERR_REGISTER_WINDOW_CLASS));
	}

	_hWindow = CreateWindowEx (
		WS_EX_ACCEPTFILES,
		WINDOW_CLASS_NAME,
		MainWindow::APPLICATION_NAME,
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		HWND_DESKTOP,
		NULL,
		hApplicationInstance,
		(void*)this //pass instance pointer (*this) to WM_NCCREATE (see onCommand)
		);

	if(_hWindow == NULL)
	{
		throw new RuntimeException("MainWindow::create", System::getLocaleString(IDERR_CREATE_WINDOW));
	}

	//register find/replace command
	_findReplaceCommandId = RegisterWindowMessage(FINDMSGSTRING);
}


void MainWindow::show(int showState)
{
	assert(_hWindow != NULL);

	_settingsInterface->applySettings();
	
	ShowWindow(_hWindow, showState);
	UpdateWindow(_hWindow);
}



// WINDOW MANAGEMENT //////////////////////////////////////////////////////////


void MainWindow::setWindowTitle(const char* newTitle)
{
	assert(_hWindow != NULL);
	SetWindowText(_hWindow, newTitle);
}


/**
 * Enables/Disables any child control that correspond to the specified command
 */
void MainWindow::setCommandEnabled(int commandId, bool isCommandEnabled)
{
	assert(_hWindow != NULL);

	// apply to menus
	HMENU mainMenu = GetMenu(_hWindow);
	int enableFlag = MF_BYCOMMAND | (isCommandEnabled ? MF_ENABLED : MF_GRAYED) ;
	EnableMenuItem(mainMenu, commandId, enableFlag);

	// apply to toolbars
	_toolbar->setCommandEnabled(commandId, isCommandEnabled);
	_staff->setCommandEnabled(commandId, isCommandEnabled);
}



// WIN32's MESSAGES HANDLING (DISPATCHERS) ////////////////////////////////////


/**
 * Win32's message handling function.
 * This function must be static in the WinAPI. But if so, it cannot access class's
 * non-static members. For convenience it retrieves a class instance previsously
 * stored in the window handle, and then forwards message to instance's WindowProc
 * (which is non-static)
 */
LRESULT CALLBACK MainWindow::forwardMessage (
	HWND hWindow,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	if (message == WM_NCCREATE)
	{
		// when creating window, store a pointer to current instance inside the window handle
		// the pointer is retrieved using lpCreateParams, which was set in CreateWindow
		SetProp(hWindow, "CorrespondingObject", ((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	// get the previously stored pointer to corresponding window instance
	MainWindow* mainWindow = (MainWindow*)GetProp(hWindow, "CorrespondingObject");

	if (mainWindow)
	{
		return mainWindow->handleMessage(hWindow, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWindow, message, wParam, lParam);
	}
}


/**
 * Win32's message handling function (the real one)
 */
LRESULT CALLBACK MainWindow::handleMessage(
	HWND hWindow,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	if(message == _findReplaceCommandId)
	{
		_editArea->onFindReplace((LPFINDREPLACE)lParam);
	}
	else switch(message)
	{
		case WM_CREATE    :  onCreate(hWindow);           break;
		case WM_CLOSE     :  onClose();                   break;
		case WM_SIZE      :  onSize();                    break;
		case WM_COMMAND   :  onCommand(wParam, lParam);   break;
		case WM_NOTIFY    :  onNotify(wParam, lParam);    break;
		case WM_DROPFILES :  onDropFiles((HDROP)wParam);  break;

		case WM_SETFOCUS  :  _editArea->setFocus();       break;

		case WM_DESTROY   :  onDestroy();                 break;

		default:
		{
			return DefWindowProc(hWindow, message, wParam, lParam);
			break;
		}
	}
	return 0;
}


void MainWindow::onCommand(WPARAM wParam, LPARAM lParam)
{
	WORD commandId = LOWORD(wParam);
	
	switch(commandId)
	{
		case ID_APP_EXIT: onClose(); break;

		case ID_FILE_NEW       : _documentInterface->onNewDocument();     break;
		case ID_FILE_SAVE      : _documentInterface->onDocumentSave();    break;
		case ID_FILE_SAVEAS    : _documentInterface->onDocumentSaveAs();  break;
		case ID_FILE_OPEN      : _documentInterface->onDocumentOpen();    break;
		case ID_FILE_PAGESETUP : _printerInterface->onChoosePageSetup();  break;
		case ID_FILE_PRINT     : _printerInterface->onPrint();            break;

		case ID_EDIT_CUT       : _editArea->doCommand(WM_CUT);     break;
		case ID_EDIT_COPY      : _editArea->doCommand(WM_COPY);    break;
		case ID_EDIT_PASTE     : _editArea->doCommand(WM_PASTE);   break;
		case ID_EDIT_UNDO      : _editArea->doCommand(WM_UNDO);    break;
		case ID_EDIT_SELECTALL : _editArea->onSelectAll();         break;
		case ID_EDIT_DELETE    : _editArea->onDelete();            break;
		case ID_EDIT_FIND      : _documentInterface->onFind();     break;
		case ID_EDIT_FINDNEXT  : _documentInterface->onFindNext(); break;
		case ID_EDIT_REPLACE   : _documentInterface->onReplace();  break;

		case ID_INSERT_STAFF  : _editArea->onInsertStaff();  break;
		case ID_INSERT_TUNING : _editArea->onInsertTuning(); break;
		case ID_INSERT_BAR    : _editArea->onInsertBar();    break;

		case ID_OPTIONS_FONT              : _settingsInterface->onChooseFont();       break;
		case ID_OPTIONS_STAFF_WIDTH       : _settingsInterface->onChooseStaffWidth(); break;
		case ID_OPTIONS_STAFF_HEIGHT      : _settingsInterface->onChooseChordDepth(); break;
		case ID_OPTIONS_TYPING_TOGGLE     : _settingsInterface->onToggleTypingMode(); break;
		case ID_OPTIONS_TYPING_INSERT     :
		case ID_OPTIONS_TYPING_OVERWRITE  :
		case ID_OPTIONS_TYPING_SPECIAL    : _settingsInterface->onChangeTypingMode(GetTypingMode(commandId)); break;
		case ID_OPTIONS_CHORD_EXTRA_SPACE :
		case ID_OPTIONS_CHORD_NAME        :
		case ID_OPTIONS_CHORD_ARPEGGIO    : _settingsInterface->onToggleChordMode(GetChordMode(commandId)); break;
		case ID_OPTIONS_EDIT_CHORDS       : 
		case ID_OPTIONS_EDIT_TUNINGS      : onEditSettingsFile(commandId);  break;

		case ID_HELP_ABOUT : AboutDialog::show(_hWindow);break;

		default:
		{
			if(commandId >= ID_OPTIONS_TUNINGS_NONE)
			{
				_settingsInterface->onChangeGuitarTuning(GetTuningIndex(commandId));
			}
			else
			{
				//DebugWindow::trace("MainWindow::onCommand(%d)", commandId);
			}
			break;
		}
	}
}


void MainWindow::onNotify(WPARAM wParam, LPARAM lParam)
{
	assert(_hWindow != NULL);

	LPNMHDR notifyMessage = (LPNMHDR)lParam;

	switch(notifyMessage->code)
	{
		case TCN_SELCHANGE: 
		{
			//maybe I should check if source window IS the chords toolbar
			_chordsTabControl->updateOnTabChange();

			//I send a "resize" message so that not-displayed-before-therefore-not-resized-but-now-displayed tabs are sized properly
			SendMessage(_hWindow, WM_SIZE, 0, 0);
			break;
		}
	}
}



// WIN32's MESSAGES HANDLING (ACTIONS) ////////////////////////////////////////


/**
 * @param hPrecreateWindow on window creation, internal pointer to window handle is not yet available, so I use the one privided by WindowProc
 */
void MainWindow::onCreate(HWND hPrecreateWindow)
{
	try
	{
		//load view
		_toolbar->create(hPrecreateWindow);
		_status->create(hPrecreateWindow);
		_staff->create(hPrecreateWindow); // must be created before _chordsTabControl, which it belongs to
		_chordsTabControl->create(hPrecreateWindow);
		_editArea->create(hPrecreateWindow);
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("MainWindow::onCreate", ex);
	}
}


void MainWindow::onClose()
{
	assert(_hWindow != NULL);

	if(_documentInterface->continueIfDocumentModified())
	{
		_settingsInterface->saveSettings();

		DestroyWindow(_hWindow);
	}
}


void MainWindow::onDestroy()
{
	PostQuitMessage(0);
}


void MainWindow::onSize()
{
	assert(_hWindow != NULL);

	//toolbar and status bar can position/resize themselves automatically
	_toolbar->resize();
	_status->resize();

	//retrieve auto-resize results
	RECT toolbarRect = _toolbar->getSize();
	RECT statusRect = _status->getSize();

	RECT clientRect;
	GetClientRect(_hWindow, &clientRect);

	//position child windows: chords toolbar
	RECT chordsTabControlRect;
	chordsTabControlRect.bottom = clientRect.bottom - (statusRect.bottom - statusRect.top);
	chordsTabControlRect.top = max(toolbarRect.bottom - toolbarRect.top + 10, chordsTabControlRect.bottom - ChordsTabControl::CHORDS_TOOLBAR_HEIGHT);
	chordsTabControlRect.left = clientRect.left;
	chordsTabControlRect.right = clientRect.right;
	_chordsTabControl->resize(chordsTabControlRect);

	//edit area
	RECT editAreaRect;
	editAreaRect.bottom = max(toolbarRect.bottom - toolbarRect.top + 10, chordsTabControlRect.bottom - ChordsTabControl::CHORDS_TOOLBAR_HEIGHT);
	editAreaRect.top = toolbarRect.bottom - toolbarRect.top;
	editAreaRect.left = clientRect.left;
	editAreaRect.right = clientRect.right;
	_editArea->resize(editAreaRect);
}


void MainWindow::onDropFiles(HDROP droppedFiles)
{
	char* fileName = new char[MAX_PATH];

	int droppedFilesCount = DragQueryFile(droppedFiles, 0xFFFFFFFF, fileName, MAX_PATH);
	if(droppedFilesCount > 0)
	{
		//get the first dropped file and open it
		DragQueryFile(droppedFiles, 0, fileName, MAX_PATH);
		_documentInterface->onDocumentOpen(fileName);
	}

	delete [] fileName;
}


/**
 * Opens the specified settings file inside tabber
 */
void MainWindow::onEditSettingsFile(WORD commandId)
{
	const char* fileName;

	if(commandId == ID_OPTIONS_EDIT_CHORDS)
	{
		fileName = _application->getChordDefinitions()->getFileName();
	}
	else
	{
		fileName = _application->getTuningDefinitions()->getFileName();
	}

	if(_documentInterface->onDocumentOpen(fileName))
	{
		NotifyMessage::alert(_hWindow, System::getLocaleString(IDS_CHANGES_ON_RESTART));
	}
}


