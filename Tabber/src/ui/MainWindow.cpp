#include "MainWindow.h"

const char CMainWindow::SZ_CLASS_NAME[] = "Tabber";


CMainWindow::CMainWindow(CApplication* lpApplication)
{
	assert(lpApplication != NULL);
	m_lpApplication = lpApplication;
	OBJECT_CREATED;
}


CMainWindow::~CMainWindow()
{
	OBJECT_DELETED;
}


/**
 * @throws RuntimeException if an initialisation step screwed up
 */
void CMainWindow::create(
	HINSTANCE hApplicationInstance,
 	int xPosition,
  	int yPosition,
   	int cxSize,
    int cySize)
{
	WNDCLASSEX wndClass;

	ZeroMemory(&wndClass, sizeof(wndClass));
    wndClass.cbSize        = sizeof(WNDCLASSEX);
    wndClass.style         = 0;
    wndClass.lpfnWndProc   = CMainWindow::WindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = hApplicationInstance;
    wndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wndClass.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wndClass.lpszClassName = SZ_CLASS_NAME;

    if(!RegisterClassEx(&wndClass))
    {
    	throw new CRuntimeException("CMainWindow::create", "Could not register class");
    }

    m_hWindow = CreateWindowEx (
        WS_EX_CLIENTEDGE,
        SZ_CLASS_NAME,
        "Untitled - Tabber",
        WS_OVERLAPPEDWINDOW,
        xPosition, yPosition, cxSize, cySize,
        HWND_DESKTOP, NULL, hApplicationInstance, NULL );

    if(m_hWindow == NULL)
    {
    	throw new CRuntimeException("CMainWindow::create", "Could not create window");
    }
}


void CMainWindow::show(int nWindowShowState)
{
	assert(m_hWindow != NULL);

    ShowWindow(m_hWindow, nWindowShowState);
    UpdateWindow(m_hWindow);
}


/**
 * Win32's message handling function
 */
LRESULT CALLBACK CMainWindow::WindowProc(
    HWND hWindow,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam )
{
    switch(uMsg)
    {
		case WM_CREATE:
        {
        	g_lpMainWindow->onCreate();
            break;
        }
        
		case WM_CLOSE:
        {
        	g_lpMainWindow->onClose();
            break;
        }
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        
    	case WM_COMMAND:
    	{
    		g_lpMainWindow->onCommand(wParam, lParam);
    		break;
    	}
        
        default:
        {
            return DefWindowProc(hWindow, uMsg, wParam, lParam);
        }
    }
    return 0;
}
 
 
void CMainWindow::onCreate()
{
	//load view
}
 

void CMainWindow::onClose()
{
	RECT rcWindow;
	CGlobalSettings*   lpSettings;
	CChordDefinitions* lpChords;

	assert(m_hWindow != NULL);
	
	//prompt to save if document has been modified
	
	//save settings
	GetWindowRect(m_hWindow, &rcWindow);
	lpSettings = m_lpApplication->getGlobalSettings();
	lpSettings->setMainWindowHeight(rcWindow.bottom - rcWindow.top);
	lpSettings->setMainWindowWidth(rcWindow.right - rcWindow.left);
	lpSettings->setMainWindowY(rcWindow.top);
	lpSettings->setMainWindowX(rcWindow.left);
	lpSettings->save();
	
	//save chords (not required for now)
	lpChords = m_lpApplication->getChordDefinitions();
	lpChords->save();
	
	DestroyWindow(m_hWindow);
}


void CMainWindow::onCommand(WPARAM wParam, LPARAM lParam)
{
	assert(m_hWindow != NULL);
	
	switch(LOWORD(wParam))
	{
		case IDC_APP_EXIT:
		{
			PostMessage(m_hWindow, WM_CLOSE, 0, 0);
			break;
		}
	}
}


