#include "PrintProgressDialog.h"

HWND PrintProgressDialog::_hWindow = NULL;
const char* PrintProgressDialog::_documentName = NULL;
char PrintProgressDialog::_status[128] = "";
 int PrintProgressDialog::_totalPages = 0;
bool PrintProgressDialog::_continuePrinting = true;



void PrintProgressDialog::show(
	HWND hParentWindow,
 	const char* documentName,
  	int totalPages )
{
	_documentName = documentName;
	_totalPages = totalPages;
	_continuePrinting = true;

    _hWindow = CreateDialog (
		GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_PRINT_DIALOG),
  		hParentWindow,
    	PrintProgressDialog::handleMessage );

	ShowWindow(_hWindow, SW_SHOW);
	UpdateWindow(_hWindow);
}


void PrintProgressDialog::hide()
{
	if(_hWindow != NULL)
	{
		onClose();
	}
}


bool PrintProgressDialog::didUserCancelPrinting()
{
	return !_continuePrinting;
}


/**
 * Updates status when new page is reached
 */
void PrintProgressDialog::setCurrentPage(int page)
{
	assert(_hWindow != NULL);

	wsprintf(_status, "Page %d / %d", page, _totalPages);
	SetDlgItemText(_hWindow, IDC_PRINT_STATUS, _status);

	UpdateWindow(_hWindow);
}


/**
 * Win32's message handling function.
 */
BOOL CALLBACK PrintProgressDialog::handleMessage(
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
			SetDlgItemText(hWindow, IDC_PRINT_DOCUMENT, _documentName);
        	break;
		}
		
        case WM_COMMAND:
      	case WM_CLOSE:
  	    {
			onClose();
  	        break;
		}
		
        default:
        {
            return FALSE;
        }    
    }
    return TRUE;
}


void PrintProgressDialog::onClose()
{
	_continuePrinting = false;
	DestroyWindow(_hWindow);
	_hWindow = NULL;
}


BOOL CALLBACK PrintProgressDialog::AbortProc(HDC printerDeviceContext, int errorCode)
{
    MSG message;

    while (_continuePrinting && PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
        if (!IsDialogMessage(_hWindow, &message))
		{
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }

     return _continuePrinting;
}


