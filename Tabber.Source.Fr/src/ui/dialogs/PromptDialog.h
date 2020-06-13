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

#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

#include "../../util/utilities.h"


class PromptDialog
{

	public:

		static int prompt(HWND , const char* , int );
		static int getIntegerValue();
		
		static BOOL CALLBACK handleMessage(HWND, UINT, WPARAM, LPARAM);
		
	protected:

		static const char* _promptString;
		static int         _returnValue;

};

#endif // PROMPTDIALOG_H

