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

#include "StaffToolbar.h"
#include "../ui/ChordsTabControl.h"
#include "../ui/MainWindow.h"


StaffToolbar::StaffToolbar(ChordsTabControl* parent)
	: ChordsTabControlPanel(parent)
{
	OBJECT_CREATED;
}


StaffToolbar::~StaffToolbar()
{
	OBJECT_DELETED;
}


void StaffToolbar::create(HWND hParentWindow)
{
	try
	{
		ChordsTabControlPanel::create(hParentWindow);
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("StaffToolbar::create", ex);
	}

	//buttons
	addButton(System::getLocaleString(IDS_INSERT_STAFF ),  ID_INSERT_STAFF,   BS_PUSHBUTTON);
	addButton(System::getLocaleString(IDS_INSERT_BAR   ),  ID_INSERT_BAR,     BS_PUSHBUTTON);
	addButton(System::getLocaleString(IDS_INSERT_TUNING),  ID_INSERT_TUNING,  BS_PUSHBUTTON);

	//subclassing (define my own window proc for this control)
	_superClassWindowProc = (WNDPROC)SetWindowLong(_hWindow, GWL_WNDPROC, (long)ChordsTabControlPanel::forwardMessage);

	//store *this pointer in window handle so that I can access class members later
	SetProp(_hWindow, "CorrespondingObject", (void*)this);
}


/**
 * @see MainWindow::handleMessage
 */
LRESULT CALLBACK StaffToolbar::handleMessage (
	HWND hWindow,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	if(message == WM_COMMAND)
	{
		SendMessage(
			_parent->getMainWindow()->getWindowHandle(),
			WM_COMMAND,
			wParam,
			lParam );
		_parent->getMainWindow()->getEditArea()->setFocus();

		return FALSE;
	}
	else
	{
		return CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);
	}
}


/**
 * Enables/Disables all the buttons that correspond to the specified command
 */
void StaffToolbar::setCommandEnabled(int commandId, bool isCommandEnabled)
{
	assert(_hWindow != NULL);

	HWND hButton = GetDlgItem(_hWindow, commandId);
	if(hButton != NULL)
	{
		EnableWindow(hButton, isCommandEnabled);
	}
}


