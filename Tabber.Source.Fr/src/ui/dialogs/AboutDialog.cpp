/*
 * Tabber - Guitar Tablature Editor, freeware and open-source
 * Copyright (C) 2004  Yvan Razafindramanana
 *
 * Additionnal Contributors:
 * <place your name here :)>
 *
 * This file is part of Tabber.
 * 
 * Tabber is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tabber is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tabber.  If not, see <https://www.gnu.org/licenses/>.
 */

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
			SetDlgItemText(hWindow, IDC_VERSION_NUMBER, __PROGRAM_VERSION__);
			SetDlgItemText(hWindow, IDC_LICENCE, System::getLocaleString(IDS_LICENCE));
			SetDlgItemText(hWindow, IDC_WEBSITE, System::getLocaleString(IDS_WEBSITE));
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

