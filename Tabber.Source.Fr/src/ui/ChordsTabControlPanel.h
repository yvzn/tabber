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

#ifndef CHORDSTABCONTROLPANEL_H
#define CHORDSTABCONTROLPANEL_H

#include "../util/utilities.h"

/* forward */ class ChordsTabControl;


/**
 * Parent class for panels displayed in the chords tab control.
 */
class ChordsTabControlPanel
{

	public:

		ChordsTabControlPanel(ChordsTabControl* );
		virtual ~ChordsTabControlPanel();

		virtual void create(HWND );
		void hide( );
		void show( );

		HWND addButton(const char* , int , DWORD =0);

		inline HWND& getWindowHandle() { return _hWindow; }

	protected:

		static  LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		virtual LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM ) =0;

		virtual int getButtonHeight  ( ) =0;
		virtual int getButtonWidth   ( ) =0;
		virtual int getButtonHorizontalPadding ( ) =0;
		virtual int getButtonVerticalPadding   ( ) =0;

	protected:

		ChordsTabControl* _parent;

		WNDPROC  _superClassWindowProc;
		HWND     _hWindow;

		int _xNextButtonPosition;
};

#endif // CHORDSTABCONTROLPANEL_H

