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

#ifndef SYSTEM_H
#define SYSTEM_H

#include <windows.h>

#include "../util/RuntimeException.h"

#define LOCALE_STRING_MAX_LENGTH 1024



/**
 * Various utilities related to system and Windows's environment
 */
class System
{

	public:

		static void    getAbsoluteFileName (char* , const char* );
		static void    applyGUIDefaultFont (HWND );

		static const char* getLocaleString(unsigned int );

	protected:

		static char _pathToExe[MAX_PATH];
		static char _localeStringBuffer[LOCALE_STRING_MAX_LENGTH];

};

#endif // SYSTEM_H

