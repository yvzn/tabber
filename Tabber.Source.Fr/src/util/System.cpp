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

#include "System.h" 

char System::_pathToExe[MAX_PATH] = "";
char System::_localeStringBuffer[LOCALE_STRING_MAX_LENGTH] = "";


/**
 * Builds an absolute path to specified file using executable's directory
 * @param absoluteFileName a string buffer where absolute filename will be stored
 * @param relativeFileName a null-terminated string representing a file name relative to current directory
 */
void System::getAbsoluteFileName(char* absoluteFileName, const char* relativeFileName)
{
	if(_pathToExe[0] == '\0') //retrieve at first call
	{
		DWORD copiedCharsCount = GetModuleFileName(NULL, _pathToExe, MAX_PATH);
		if(copiedCharsCount == 0)
		{
			throw new RuntimeException("::GetAbsoluteFileName", "Could not find executable location");
		}

		//find the last "\\" (limit between path and .exe name)
		int limit;
		for(limit = copiedCharsCount-1; _pathToExe[limit] != '\\' && limit > -1; limit--)
			;

		//cut string down to path
		_pathToExe[limit] = '\0';
	}

	lstrcpy(absoluteFileName, _pathToExe);
	lstrcat(absoluteFileName, "\\");
	lstrcat(absoluteFileName, relativeFileName);
}


/**
 * Applies the default GUI font (as specified by the user in Windows's display settings) to a control
 */
void System::applyGUIDefaultFont(HWND hControl)
{
	static HFONT defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	SendMessage(hControl, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE, 0));
}


/**
 * Loads the localized string for the specified resource identifier / command Id
 * @warning returned pointer is volatile, client should copy data if it needs a long term use.
 */
const char* System::getLocaleString(unsigned int resourceId)
{
	LoadString(
		GetModuleHandle(NULL),
		resourceId,
		_localeStringBuffer,
		LOCALE_STRING_MAX_LENGTH );

	return _localeStringBuffer;
}

