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

#ifndef CHORDSTOOLBAR_H
#define CHORDSTOOLBAR_H

#include "../util/utilities.h"
#include "../ui/ChordsTabControlPanel.h"
#include "../ui/tools/EditionActions.h"


/**
 * A basic container for buttons, with the ability to process button clicks.
 */
class ChordsToolbar: public ChordsTabControlPanel
{

	public:

		ChordsToolbar(ChordsTabControl* );
		~ChordsToolbar();

		void create(HWND );

		HWND addButton(const char* , int , DWORD =0);

	protected:

		LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		inline int getButtonHeight  ( ) { return 25; }
		inline int getButtonWidth   ( ) { return 50; }
		virtual int getButtonHorizontalPadding ( ) { return 2; }
		virtual int getButtonVerticalPadding   ( ) { return 2; }

};


#endif // CHORDSTOOLBAR_H

