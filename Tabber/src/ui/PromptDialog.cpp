#include "PromptDialog.h"

const char* PromptDialog::_promptString = NULL;
        int PromptDialog::_returnValue = 0;


/**
 * Displays a prompt dialog where user is invited to enter a value for a parameter
 * @return IDOK if user changed the value and clicked OK button
 * @return IDCANCE if user cancelled operation
 * @warning To get the value entered by the user, you should use getIntegerValue
 */
int PromptDialog::prompt(
	HWND hParentWindow,
 	const char* promptString,
  	int defaultValue )
{
	_promptString = promptString;
	_returnValue = defaultValue;

    return DialogBox(
     	GetModuleHandle(NULL), 
		MAKEINTRESOURCE(IDD_PROMPT_DIALOG),
  		hParentWindow,
    	PromptDialog::handleMessage);
}    


/**
 * Retrieves the value entered by user in the prompt dialog
 * @warning You should call this function immediately after calling prompt method, otherwise the value provided is not guaranteed
 */
int PromptDialog::getIntegerValue()
{
	return _returnValue;
}


/**
 * Win32's message handling function.
 */
BOOL CALLBACK PromptDialog::handleMessage(
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
    switch(message)
    {
        case WM_INITDIALOG:
        {
			SetDlgItemText(hWindow, IDC_PROMPT_STRING, _promptString);
			SetDlgItemInt(hWindow, IDC_PROMPT_VALUE, _returnValue, FALSE);
        	break;
		}
		
		case WM_SETFOCUS: // does not work
		{
			HWND hValue = GetDlgItem(hWindow, IDC_PROMPT_VALUE);
			SendMessage(hValue, EM_SETSEL, 0, (LPARAM)-1);
			SetFocus(hValue);
			break;
		}
      
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDOK:
                {
                    BOOL ok;
                    int val = GetDlgItemInt(hWindow, IDC_PROMPT_VALUE, &ok, FALSE);
                    if(ok) _returnValue = val;
                    
                    EndDialog(hWindow, IDOK);
                	break;
         		}
         		
         		case IDCANCEL:
                {
  			        EndDialog(hWindow, IDCANCEL);
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

