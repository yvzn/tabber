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

#include "ChordsToolbar.h"
#include "../ui/ChordsTabControl.h"
#include "../ui/MainWindow.h"


ChordsToolbar::ChordsToolbar(ChordsTabControl* parent)
	: ChordsTabControlPanel(parent)
{
	OBJECT_CREATED;
}


ChordsToolbar::~ChordsToolbar()
{
	OBJECT_DELETED;
}


void ChordsToolbar::create(HWND hParentWindow)
{
	try
	{
		ChordsTabControlPanel::create(hParentWindow);
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("ChordsToolbar::create", ex);
	}
}


HWND ChordsToolbar::addButton(const char* buttonLabel, int buttonId, DWORD buttonStyle)
{
	assert(_hWindow != NULL);

	HWND hButton = ChordsTabControlPanel::addButton(buttonLabel, buttonId, buttonStyle | BS_PUSHBUTTON);

	//subclassing (define my own window proc for this control)
	_superClassWindowProc = (WNDPROC)SetWindowLong(hButton, GWL_WNDPROC, (long)ChordsTabControlPanel::forwardMessage);

    //store *this pointer in window handle so that I can access class members later
    SetProp(hButton, "CorrespondingObject", (void*)this);

	return hButton;
}


/**
 * @see MainWindow::handleMessage
 */
LRESULT CALLBACK ChordsToolbar::handleMessage (
    HWND hWindow,
    UINT message,
    WPARAM wParam,
    LPARAM lParam )
{
	if(message == WM_LBUTTONUP)
	{
		unsigned int commandId = GetDlgCtrlID(hWindow);
		EditArea* editArea = _parent->getMainWindow()->getEditArea();

		if(wParam & MK_CONTROL)
		{
			editArea->onInsertChord(commandId, ArpeggioDispatcher::ASCENDING);
		}
		else if(wParam & MK_SHIFT)
		{
			editArea->onInsertChord(commandId, ArpeggioDispatcher::DESCENDING);
		}
 		else
		{
   			editArea->onInsertChord(commandId);
		}
	}

	return CallWindowProc(_superClassWindowProc, hWindow, message, wParam, lParam);
}


