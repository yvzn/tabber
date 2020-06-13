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

#include "StatusBar.h"


StatusBar::StatusBar()
{
	_typingModeString = new char[11];
	OBJECT_CREATED;
}


StatusBar::~StatusBar()
{
	delete [] _typingModeString;
	OBJECT_DELETED;
}


void StatusBar::create(HWND hParentWindow)
{
	_hWindow = CreateWindowEx (
		0,
		STATUSCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		0, 0, 0, 0,
		hParentWindow, (HMENU)IDC_MAIN_STATUSBAR, GetModuleHandle(NULL), NULL );

	if(_hWindow == NULL)
	{
		throw new RuntimeException("StatusBar::create", System::getLocaleString(IDERR_CREATE_WINDOW));	
	}

	int partsRightBorderPositions[] = {70, -1};
	SendMessage(_hWindow, SB_SETPARTS, sizeof(partsRightBorderPositions)/sizeof(int), (LPARAM)partsRightBorderPositions);
}


void StatusBar::resize()
{
	assert(_hWindow != NULL);
	//StatusBar can resize itself automatically inside parent window
	SendMessage(_hWindow, WM_SIZE, 0, 0);
}


RECT StatusBar::getSize()
{
	assert(_hWindow != NULL);
	
	RECT windowRect;
	GetWindowRect(_hWindow, &windowRect);
	
	return windowRect;
}


void StatusBar::setTextInPart(int indexOfPart, const char* newPartText)
{
	assert(_hWindow != NULL);
	PostMessage(_hWindow, SB_SETTEXT, (WPARAM)indexOfPart, (LPARAM)newPartText);
}


void StatusBar::updateTypingMode(TypingMode typingMode)
{
	switch(typingMode)
	{
		case OVERWRITE:
		{
			lstrcpy(_typingModeString, System::getLocaleString(IDS_TYPE_OVERWRITE));
			break;
		}

		case SPECIAL:
		{
			lstrcpy(_typingModeString, System::getLocaleString(IDS_TYPE_SPECIAL));
			break;
		}

		default:
		{
			lstrcpy(_typingModeString, System::getLocaleString(IDS_TYPE_INSERT));
			break;
		}
	}
	setTextInPart(0, _typingModeString);
}

