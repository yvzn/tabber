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

#include <windows.h>

#include "core/Application.h"
#include "util/utilities.h"


DEBUG_START;

int WINAPI WinMain (
	HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow )
{
	MSG msg;
	Application* application = new Application();

	try
	{
		application->create(hThisInstance);
		application->show(nCmdShow);
		application->processCommandLine(lpCmdLine);
	
		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
			if(!application->interceptsMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	catch(RuntimeException* exception)
	{
		RuntimeException ex("::Winmain", exception);
		NotifyMessage::publicError(HWND_DESKTOP, ex.getStackTrace());
	}
	
	delete application;

DEBUG_STOP;

	return msg.wParam;
}
