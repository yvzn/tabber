#include "AboutDialog.h"


int AboutDialog::show(HWND hParentWindow)
{
     return DialogBox(
     	GetModuleHandle(NULL), 
		MAKEINTRESOURCE(IDD_ABOUT_DIALOG),
  		hParentWindow,
    	AboutDialog::handleMessage);
}    


/**
 * Win32's message handling function.
 */
BOOL CALLBACK AboutDialog::handleMessage(
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
			SetDlgItemText(hWindow, IDC_VERSION_NUMBER, __DATE__ " at " __TIME__);
        	break;
		}
      
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDOK:
                {
                    EndDialog(hWindow, IDOK);
                	break;
         		}   	
            }
        	break;
 		}
      	
      	case WM_CLOSE:
  	    {
  	        EndDialog(hWindow, IDCANCEL);
  	        break;
		}
		
        default:
        {
            return FALSE;
        }    
    }
    return TRUE;
}    

