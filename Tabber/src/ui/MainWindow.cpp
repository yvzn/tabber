#include "MainWindow.h"

const char MainWindow::WINDOW_CLASS_NAME[] = "MainWindow";
const char MainWindow::APPLICATION_NAME[] = "Tabber";



// CONSTR AND DESTR ///////////////////////////////////////////////////////////


MainWindow::MainWindow(Application* application)
{
	InitCommonControls();

	_application     = application;
	_toolbar         = new MainToolbar();
	_status          = new StatusBar();
	_chordsToolbar   = new ChordsToolbar(this);
	_editArea        = new EditArea(this);
	_documentInterface = new DocumentInterface(this);
	_settingsInterface = new SettingsInterface(this);
	
	OBJECT_CREATED;
}


MainWindow::~MainWindow()
{
	delete _settingsInterface;
	delete _documentInterface;
	delete _editArea;
	delete _chordsToolbar;
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
	const RECT windowRect = _application->getSettings()->getMainWindowRect();
	int x, y, width, height;
	if(windowRect.top < 0)
	{
		// rect is not valid, use defaults values	
		x = y = width = height = CW_USEDEFAULT;
	}
	else
	{
		x = windowRect.left; width = windowRect.right - windowRect.left;
		y = windowRect.top; height = windowRect.bottom - windowRect.top;
	}

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
    	throw new RuntimeException("MainWindow::create", "Could not register class");
    }

    _hWindow = CreateWindowEx (
        0,
        WINDOW_CLASS_NAME,
        "Untitled - Tabber",
        WS_OVERLAPPEDWINDOW, 
        x, y, width, height,
        HWND_DESKTOP,
        NULL,
        hApplicationInstance,
        (void*)this //pass *this pointer to WM_NCCREATE (see onCommand)
        );

    if(_hWindow == NULL)
    {
    	throw new RuntimeException("MainWindow::create", "Could not create main window");
    }
}


void MainWindow::show(int showState)
{
	assert(_hWindow != NULL);

    ShowWindow(
    	_hWindow,
 		_application->getSettings()->getMainWindowMaximizedState() ? SW_MAXIMIZE : showState );
    UpdateWindow(_hWindow);
}



// WINDOW MANAGEMENT //////////////////////////////////////////////////////////


void MainWindow::setWindowTitle(const char* newTitle)
{
	assert(_hWindow != NULL);
	SetWindowText(_hWindow, newTitle);
}


/**
 * Enables/Disables all the child controls that correspond to the specified command
 */
void MainWindow::setCommandEnabled(int commandId, bool isCommandEnabled)
{
    assert(_hWindow != NULL);
    
    //menus
    HMENU mainMenu = GetMenu(_hWindow);
    int enableFlag = MF_BYCOMMAND | (isCommandEnabled ? MF_ENABLED : MF_GRAYED) ;
	EnableMenuItem(mainMenu, commandId, enableFlag);
    
    //toolbar
    _toolbar->setCommandEnabled(commandId, isCommandEnabled);
}



// WIN32's MESSAGES HANDLING //////////////////////////////////////////////////


/**
 * Win32's message handling function.
 * This function must be static in the WinAPI. But if so, it cannot access class
 * members. For convenience it then forwards message to class's real WindowProc.
 */
LRESULT CALLBACK MainWindow::forwardMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
    if (message == WM_NCCREATE)
    {
        // when creating window, store the pointer to the window
        // from lpCreateParams which was set in CreateWindow
        SetProp(hWindow, "CorrespondingObject", ((LPCREATESTRUCT(lParam))->lpCreateParams));
    }

    // get the previously stored pointer to the window
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
    switch(message)
    {
		case WM_CREATE:
        {
        	onCreate(hWindow);
            break;
        }
        
		case WM_CLOSE:
        {
        	onClose();
            break;
        }
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        
        case WM_SIZE:
        {
        	onSize();
        	break;
        }
        
        case WM_SETFOCUS:
        {
			_editArea->setFocus();
        	break;   
        }    
        
    	case WM_COMMAND:
    	{
    		onCommand(wParam, lParam);
    		break;
    	}
        
        case WM_NOTIFY:
        {
    		onNotify(wParam, lParam);
    		break;
        }
        
        default:
        {
            return DefWindowProc(hWindow, message, wParam, lParam);
        }
    }
    return 0;
}
 
 
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
		_chordsToolbar->create(hPrecreateWindow);
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
		//save settings
		RECT windowRect;
		GetWindowRect(_hWindow, &windowRect);
	
		ApplicationSettings* appSettings = _application->getSettings();
		appSettings->setMainWindowRect(windowRect);
		appSettings->setMainWindowMaximizedState(IsZoomed(_hWindow) > 0);
		appSettings->save();
	
		DestroyWindow(_hWindow);
	}
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
 	RECT chordsToolbarRect;
 	chordsToolbarRect.bottom = clientRect.bottom - (statusRect.bottom - statusRect.top);
 	chordsToolbarRect.top = max(toolbarRect.bottom - toolbarRect.top + 10, chordsToolbarRect.bottom - ChordsToolbar::CHORDS_TOOLBAR_HEIGHT);
 	chordsToolbarRect.left = clientRect.left;
 	chordsToolbarRect.right = clientRect.right;
	_chordsToolbar->resize(chordsToolbarRect);
	
	//edit area
	RECT editAreaRect;
	editAreaRect.bottom = max(toolbarRect.bottom - toolbarRect.top + 10, chordsToolbarRect.bottom - ChordsToolbar::CHORDS_TOOLBAR_HEIGHT);
	editAreaRect.top = toolbarRect.bottom - toolbarRect.top;
	editAreaRect.left = clientRect.left;
 	editAreaRect.right = clientRect.right;
	_editArea->resize(editAreaRect);
}


void MainWindow::onCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
		case ID_APP_EXIT:
		{
			onClose();
			break;
		}
		
		case ID_FILE_NEW:
		{
			_documentInterface->onNewDocument();
			break;
		}

		case ID_FILE_SAVE:
		{
			_documentInterface->onDocumentSave();
			break;
		}
		
		case ID_FILE_SAVEAS:
		{
			_documentInterface->onDocumentSaveAs();
			break;
		}
		
		case ID_FILE_OPEN:
		{
			_documentInterface->onDocumentOpen();
			break;
		}
		
		case ID_EDIT_CUT:
		{
			_editArea->doCommand(WM_CUT);
			break;
		}

		case ID_EDIT_COPY:
		{
			_editArea->doCommand(WM_COPY);
			break;
		}

		case ID_EDIT_PASTE:
		{
			_editArea->doCommand(WM_PASTE);
			break;
		}

		case ID_EDIT_UNDO:
		{
			_editArea->doCommand(WM_UNDO);
			break;
		}
		
		case ID_OPTIONS_FONT:
		{
		    _settingsInterface->onChooseFont();
		    break;
		}
      
		case ID_HELP_ABOUT:
		{
		    AboutDialog::show(_hWindow);
		    break;
		}
      
		default:
		{
			if(LOWORD(wParam) > IDC_FIRST_CHORD)
				DebugWindow::trace("MainWindow Command => %d ", LOWORD(wParam));
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
			_chordsToolbar->updateOnTabChange();
			
			//I send a "resize" message so that not-displayed-before-therefore-not-resized-but-now-displayed tabs are sized properly
			SendMessage(_hWindow, WM_SIZE, 0, 0);
			break;
		}
	}
}



