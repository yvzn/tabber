#include "DebugWindow.h"
#include "../util/RuntimeException.h"

const char DebugWindow::WINDOW_CLASS_NAME[] = "DebugWindow";
HWND       DebugWindow::_hWindow = NULL;
bool       DebugWindow::_isWindowClassNotRegistered = true;

/**
 * @throws RuntimeException if an initialisation step screwed up
 */
void DebugWindow::create(HINSTANCE hApplicationInstance)
{
	if(_isWindowClassNotRegistered)
	{
    	WNDCLASSEX wndClass;
    	ZeroMemory(&wndClass, sizeof(wndClass));
        wndClass.cbSize        = sizeof(WNDCLASSEX);
        wndClass.lpfnWndProc   = DebugWindow::handleMessage;
        wndClass.hInstance     = hApplicationInstance;
        wndClass.hIcon         = LoadIcon(hApplicationInstance, MAKEINTRESOURCE(IDI_ICON_LARGE));
        wndClass.hIconSm       = LoadIcon(hApplicationInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));
        wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);
        wndClass.lpszClassName = DebugWindow::WINDOW_CLASS_NAME;
    
        if(!RegisterClassEx(&wndClass))
        {
			throw new RuntimeException("DebugWindow::create", System::getLocaleString(IDERR_REGISTER_WINDOW_CLASS));
        }
        _isWindowClassNotRegistered = false;
	}    

    _hWindow = CreateWindowEx (
        0,
        DebugWindow::WINDOW_CLASS_NAME,
        "Tabber - Debug Trace",
        WS_OVERLAPPEDWINDOW, 
        50, 50, 400, 500,
        HWND_DESKTOP,
        NULL,
        hApplicationInstance,
        NULL );

    if(_hWindow == NULL)
    {
		throw new RuntimeException("DebugWindow::create", System::getLocaleString(IDERR_CREATE_WINDOW));
    }
    
    ShowWindow(_hWindow, SW_SHOWNOACTIVATE);
    UpdateWindow(_hWindow);
}


/**
 * Win32's message handling function.
 */
LRESULT CALLBACK DebugWindow::handleMessage(
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
    switch(message)
    {
        case WM_CREATE:
        {
            HWND hEdit = CreateWindowEx (
       			WS_EX_CLIENTEDGE,
        		"EDIT",
        		NULL,
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
  				ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
        		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        		hWindow, (HMENU)IDC_DEBUG_OUTPUT, GetModuleHandle(NULL), NULL
        		);
        		
    		if(hEdit == NULL)
    		{
		    	throw new RuntimeException("DebugWindow::handleMessage", System::getLocaleString(IDERR_CREATE_WINDOW));
    		}
    		
			System::applyGUIDefaultFont(hEdit);
		    
    		break;
		}

		case WM_SIZE:
  		{
  	        RECT clientRect;
	        GetClientRect(hWindow, &clientRect);
	        
		 	HWND hEdit = GetDlgItem(hWindow, IDC_DEBUG_OUTPUT);
	        SetWindowPos(hEdit, NULL, 0, 0, clientRect.right, clientRect.bottom, SWP_NOZORDER);
	        break;
  		}    
      	
      	case WM_CLOSE:
  	    {
  	        DestroyWindow(hWindow);
  	        break;
		}
		
		case WM_DESTROY:
		{
			_hWindow = NULL;
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
 * Displays a formatted string
 * @see wvsprintf
 */
void DebugWindow::trace(const char* format, ...)
{
    va_list ap;
    char* debugBuffer = new char[1000];
    
    va_start(ap, format);
    wvsprintf(debugBuffer, format, ap );
    va_end(ap);

	writeln(debugBuffer);

	delete [] debugBuffer;
}


/**
 * Formats and displays Windows' last error message
 */
void DebugWindow::traceWindowsLastError()
{
    LPSTR lpMsgBuf;
    
    FormatMessage(
    	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    	NULL,
     	GetLastError(),
    	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    	(LPSTR) &lpMsgBuf,
    	0,
    	NULL 
	);

	writeln(lpMsgBuf);
	LocalFree( lpMsgBuf );
}


void DebugWindow::writeln(const char* text)
{
	if(_hWindow == NULL)
	{
 		try
 		{
		    create(GetModuleHandle(NULL));
		} 
		catch(RuntimeException* source)
		{
			RuntimeException ex("DebugWindow::debug", source);
			NotifyMessage::publicError(HWND_DESKTOP, ex.getStackTrace());
		}    
	} 
    
	//pre-pend the text at the beginning of output area
	HWND hOutput = GetDlgItem(_hWindow, IDC_DEBUG_OUTPUT);
	int end = SendMessage(hOutput, WM_GETTEXTLENGTH, 0, 0);
	SendMessage(hOutput, EM_SETSEL, end, end);
	SendMessage(hOutput, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)text);	
	char CRLF[] = "\r\n";
	SendMessage(hOutput, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)CRLF);	
}


void DebugWindow::alert(const char* format, ...)
{
    va_list ap;
    char* buffer = new char[1000];
    
    va_start(ap, format);
    wvsprintf(buffer, format, ap );
    va_end(ap);
    
	MessageBox (
		HWND_DESKTOP,
		buffer,
		"Debug : Notification",
		MB_ICONEXCLAMATION | MB_OK );    
		
	delete [] buffer;
}


/**
 * Copies window's content to the clip board
 */
void DebugWindow::copyTrace()
{
    assert(_hWindow != NULL);
    
	HWND hEdit = GetDlgItem(_hWindow, IDC_DEBUG_OUTPUT);
	SendMessage(hEdit, EM_SETSEL, 0, -1);
	SendMessage(hEdit, WM_COPY, 0, 0);
}
