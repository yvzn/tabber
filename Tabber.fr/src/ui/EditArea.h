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

#ifndef EDITAREA_H
#define EDITAREA_H

#include "../util/utilities.h"
#include "../core/ApplicationSettings.h"
#include "../ui/tools/EditionToolkit.h"
#include "../ui/tools/EditionActions.h"

/* forward */ class MainWindow;


class EditArea
{

	public:

		EditArea(MainWindow* );
		~EditArea();

		void create (HWND );
		void resize (const RECT& );

		void loadContentFrom (const char* );
		void saveContentTo   (const char* );
		void wipeContent     ( );

		void doCommand (UINT );

		void setFont       (const LOGFONT& );
		void setFocus      ( )              ;

		void onSelectAll    ( );
		void onDelete       ( );
		void onInsertStaff  ( );
		void onInsertTuning ( );
		void onInsertBar    ( );
		void onInsertChord  (unsigned int , ArpeggioDispatcher::Direction =ArpeggioDispatcher::NONE);

		void onFindReplace  (LPFINDREPLACE );
		
		void onSelectionChange ( );

		inline MainWindow*     getMainWindow () { return _mainWindow;  }
		inline EditionToolkit* getToolkit    () { return _toolkit;     }

		inline HWND   getWindowHandle () { return _hWindow;     }
		inline HFONT  getDisplayFont  () { return _displayFont; }

	protected:

		static LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		       LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		void    onDocumentModified  ( );
		LRESULT onKeyUp             (int );
		LRESULT onKeyDown           (int );
		LRESULT onCharSpecialMode   (int );
		LRESULT onCharOverwriteMode (int);

		void apply(StaffAction *);

	protected:

		MainWindow* _mainWindow;
		
		WNDPROC  _superClassWindowProc;
		HWND     _hWindow;
		HFONT    _displayFont;
		
		EditionToolkit* _toolkit;

		bool _isSearchedTextSelected;

};


#endif // EDITAREA_H

