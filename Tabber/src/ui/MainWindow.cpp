#include "MainWindow.h"

const char MainWindow::WINDOW_CLASS_NAME[] = "MainWindow";
const char MainWindow::APPLICATION_NAME[] = "Tabber";


MainWindow::MainWindow(Application* application)
{
	InitCommonControls();

	_application     = application;
	_toolbar         = new MainToolbar();
	_status          = new StatusBar();
	_chordsToolbar   = new ChordsToolbar(this);
	_editArea        = new EditArea(this);
	_documentManager = new DocumentManager(this);
	
	OBJECT_CREATED;
}


MainWindow::~MainWindow()
{
	delete _documentManager;
	delete _editArea;
	delete _chordsToolbar;
	delete _toolbar;
	delete _status;
	OBJECT_DELETED;
}


/**
 * @throws RuntimeException if an initialisation step screwed up
 */
void MainWindow::create(HINSTANCE hApplicationInstance)
{
	ApplicationSettings* settings = _application->getSettings();
	
	const RECT windowRect = settings->getMainWindowRect();
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

	DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	if(settings->getMainWindowMaximizedState())
	{
		windowStyle |= WS_MAXIMIZE;
	}
	
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
    wndClass.cbSize        = sizeof(WNDCLASSEX);
    //wndClass.style         = 0;
    wndClass.lpfnWndProc   = MainWindow::forwardMessage;
    //wndClass.cbClsExtra    = 0;
    //wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = hApplicationInstance;
    wndClass.hIcon         = LoadIcon(hApplicationInstance, MAKEINTRESOURCE(IDI_ICON_LARGE));
    wndClass.hIconSm       = LoadIcon(hApplicationInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));
    wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);
    wndClass.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wndClass.lpszClassName = WINDOW_CLASS_NAME;

    if(!RegisterClassEx(&wndClass))
    {
    	throw new RuntimeException("MainWindow::create", "Could not register class");
    }

    _hWindow = CreateWindowEx (
        0,
        WINDOW_CLASS_NAME,
        "Untitled - Tabber",
        windowStyle, 
        x, y, width, height,
        HWND_DESKTOP,
        NULL,
        hApplicationInstance,
        (void*)this //store *this pointer in window handle so that I can access class variables and methods
        );

    if(_hWindow == NULL)
    {
    	throw new RuntimeException("MainWindow::create", "Could not create main window");
    }
    
    //SetWindowLong(_hWindow, GWL_USERDATA, (long)this);
}


void MainWindow::show(int showState)
{
	assert(_hWindow != NULL);

    ShowWindow(_hWindow, showState);
    UpdateWindow(_hWindow);
}


Application* MainWindow::getApplication()
{
	return _application;
}


HWND& MainWindow::getWindowHandle()
{
	return _hWindow;
}


void MainWindow::setWindowTitle(const char* newTitle)
{
	assert(_hWindow != NULL);
	SetWindowText(_hWindow, newTitle);
}


EditArea* MainWindow::getEditArea()
{
	return _editArea;
}


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
	
	if(_documentManager->continueIfDocumentModified())
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
			_documentManager->onNewDocument();
			break;
		}

		case ID_FILE_SAVE:
		{
			_documentManager->onDocumentSave();
			break;
		}
		
		case ID_FILE_SAVEAS:
		{
			_documentManager->onDocumentSaveAs();
			break;
		}
		
		case ID_FILE_OPEN:
		{
			_documentManager->onDocumentOpen();
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
		
		default:
		{
			if(LOWORD(wParam) > IDC_FIRST_CHORD)
				NotifyMessage::debug("MainWindow Command => %d ", LOWORD(wParam));
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


