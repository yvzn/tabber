#include "MainWindow.h"

const char MainWindow::WINDOW_CLASS_NAME[] = "Tabber";


MainWindow::MainWindow(Application* application)
{
	InitCommonControls();

	_application = application;
	_toolbar     = new MainToolbar();
	_status      = new StatusBar();
	
	OBJECT_CREATED;
}


MainWindow::~MainWindow()
{
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
	assert(settings != NULL);
	
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
    wndClass.style         = 0;
    wndClass.lpfnWndProc   = MainWindow::WindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = hApplicationInstance;
    wndClass.hIcon         = LoadIcon(hApplicationInstance, MAKEINTRESOURCE(IDI_ICON_LARGE));
    wndClass.hIconSm       = LoadIcon(hApplicationInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));
    wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wndClass.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wndClass.lpszClassName = WINDOW_CLASS_NAME;

    if(!RegisterClassEx(&wndClass))
    {
    	throw new RuntimeException("MainWindow::create", "Could not register class");
    }

    _hWindow = CreateWindowEx (
        0,
        WINDOW_CLASS_NAME,
        "Tabber",
        windowStyle, 
        x, y, width, height,
        HWND_DESKTOP, NULL, hApplicationInstance, NULL );

    if(_hWindow == NULL)
    {
    	throw new RuntimeException("MainWindow::create", "Could not create main window");
    }
}


void MainWindow::show(int showState) const
{
	assert(_hWindow != NULL);

    ShowWindow(_hWindow, showState);
    UpdateWindow(_hWindow);
}


/**
 * Win32's message handling function
 */
LRESULT CALLBACK MainWindow::WindowProc(
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
	assert(gMainWindow != NULL);

    switch(message)
    {
		case WM_CREATE:
        {
        	gMainWindow->onCreate(hWindow);
            break;
        }
        
		case WM_CLOSE:
        {
        	gMainWindow->onClose();
            break;
        }
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        
        case WM_SIZE:
        {
        	gMainWindow->onSize();
        	break;
        }
        
    	case WM_COMMAND:
    	{
    		gMainWindow->onCommand(wParam, lParam);
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
 * @param hPrecreateWindow on window creation, internal pointer is not yet available, so I use this temporary one
 */
void MainWindow::onCreate(HWND hPrecreateWindow)
{
	try
	{
		//load view
		_toolbar->create(hPrecreateWindow);
		_status->create(hPrecreateWindow);
		_status->setTextInPart(0, "Hello World !");
		
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("MainWindow::onCreate", ex);
	}
}
 

void MainWindow::onClose()
{
	assert(_hWindow != NULL);
	
	//prompt to save if document has been modified
	
	//save settings
	RECT rcWindow;
	GetWindowRect(_hWindow, &rcWindow);
	
	ApplicationSettings* appSettings = _application->getSettings();
	appSettings->setMainWindowRect(rcWindow);
	appSettings->setMainWindowMaximizedState(IsZoomed(_hWindow) > 0);
	appSettings->save();
	
	DestroyWindow(_hWindow);
}


void MainWindow::onSize()
{
	//toolbar and status bar can position/resize themselves automatically
	_toolbar->resize();
	_status->resize();
}


void MainWindow::onCommand(WPARAM wParam, LPARAM lParam)
{
	assert(_hWindow != NULL);
	
	switch(LOWORD(wParam))
	{
		case ID_APP_EXIT:
		{
			PostMessage(_hWindow, WM_CLOSE, 0, 0);
			break;
		}
	}
}



