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

#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <windows.h>
#include "../resource.h"
#include "../util/NotifyMessage.h"


class DebugWindow
{

	public:

		static void trace(const char* , ...);
		static void traceWindowsLastError();
		static void alert(const char*, ...);

		static void copyTrace();

	protected:

		static const char WINDOW_CLASS_NAME[];

		static void create(HINSTANCE );
		static LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );
		static void writeln(const char* );

		static HWND _hWindow;
		static bool _isWindowClassNotRegistered;

};


#define __trace DebugWindow::trace
#define __alert DebugWindow::alert


#endif // DEBUGWINDOW_H

