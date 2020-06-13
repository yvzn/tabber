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

#include "NotifyMessage.h"


#define DECLARE_MESSENGER_BODY(icon, hWindow)   \
{                                               \
    va_list ap;                                 \
    char* buffer = new char[1000];              \
                                                \
    va_start(ap, format);                       \
    wvsprintf(buffer, format, ap );             \
    va_end(ap);                                 \
                                                \
    MessageBox (                                \
        hWindow,                                \
        buffer,                                 \
        "Tabber",                               \
        icon | MB_OK );                         \
                                                \
    delete [] buffer;                           \
}


void NotifyMessage::publicError(HWND hParentWindow, const char* format, ...)
DECLARE_MESSENGER_BODY(MB_ICONERROR, hParentWindow);


void NotifyMessage::alert(HWND hParentWindow, const char* format, ...)
DECLARE_MESSENGER_BODY(MB_ICONINFORMATION, hParentWindow);


int NotifyMessage::assertionFailed(const char* expr, const char* file, int line)
{
	char* buffer = new char[1000];
	wsprintf(buffer, "The followong assertion failed.\n\n%s\n\nFile: %s\nLine: %d", expr, file, line);

	int result = MessageBox (
		HWND_DESKTOP,
		buffer,
		"Debug : Assertion Failed !",
		MB_ICONERROR | MB_ABORTRETRYIGNORE );

	delete [] buffer;
	return result;
}

