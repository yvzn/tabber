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

