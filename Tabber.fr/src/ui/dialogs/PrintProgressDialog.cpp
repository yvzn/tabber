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


