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

#include "ChordsTabControlPanel.h"


ChordsTabControlPanel::ChordsTabControlPanel(ChordsTabControl* parent)
{
	_parent = parent;
	_xNextButtonPosition = 0;
	OBJECT_CREATED;
}

ChordsTabControlPanel::~ChordsTabControlPanel()
{
	OBJECT_DELETED;
}


void ChordsTabControlPanel::create(HWND hParentWindow)
{
	_hWindow = CreateWindowEx(
		0,
		"STATIC",
		NULL,
		WS_CHILD,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hParentWindow,
		NULL,
		GetModuleHandle(NULL),
		(void*)this );

	if(_hWindow == NULL)
	{
		throw new RuntimeException("ChordsToolbar::create", System::getLocaleString(IDERR_CREATE_WINDOW));
	}
}


void ChordsTabControlPanel::show()
{
	assert(_hWindow != NULL);
	ShowWindow(_hWindow, SW_SHOW);
}


void ChordsTabControlPanel::hide()
{
	assert(_hWindow != NULL);
	ShowWindow(_hWindow, SW_HIDE);
}


/**
 * @see MainWindow::forwardMessage
 */
LRESULT CALLBACK ChordsTabControlPanel::forwardMessage (
	HWND hWindow,
	UINT message,
	WPARAM wParam,
	LPARAM lParam )
{
	//retrieve *this pointer and then forward message
	ChordsTabControlPanel* panel = (ChordsTabControlPanel*)GetProp(hWindow, "CorrespondingObject");
	if (panel)
	{
		return panel->handleMessage(hWindow, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWindow, message, wParam, lParam);
	}
}


HWND ChordsTabControlPanel::addButton(const char* buttonLabel, int buttonId, DWORD buttonStyle)
{
	assert(_hWindow != NULL);

	HWND hButton = CreateWindowEx(
		0,
		"BUTTON",
		buttonLabel,
		WS_CHILD | WS_VISIBLE | buttonStyle,
		_xNextButtonPosition,
		getButtonVerticalPadding(),
		getButtonWidth(),
		getButtonHeight(),
		_hWindow,
		(HMENU)buttonId,
		GetModuleHandle(NULL),
		NULL );

	if(hButton == NULL)
	{
		throw new RuntimeException("ChordsTabControlPanel::addButton", System::getLocaleString(IDERR_CREATE_WINDOW));
	}


	System::applyGUIDefaultFont(hButton);

	_xNextButtonPosition += getButtonWidth() + getButtonHorizontalPadding();

	return hButton;
}

