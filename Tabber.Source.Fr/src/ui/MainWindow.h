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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../util/utilities.h"

#include "../core/Application.h"
#include "../core/ApplicationSettings.h"

#include "../ui/MainToolbar.h"
#include "../ui/StatusBar.h"
#include "../ui/ChordsTabControl.h"
#include "../ui/EditArea.h"
#include "../ui/StaffToolbar.h"

#include "../ui/tools/DocumentInterface.h"
#include "../ui/tools/SettingsInterface.h"
#include "../ui/tools/PrinterInterface.h"

#include "../ui/dialogs/AboutDialog.h"




class MainWindow
{

	public:

		MainWindow(Application *);
		virtual ~MainWindow();

		void create(HINSTANCE ) ;
		void show  (int ) ;

		void setWindowTitle    (const char* );
		void setCommandEnabled (int , bool );

		inline bool interceptsMessage(MSG* message) { return _documentInterface->interceptsMessage(message); }

		inline Application*       getApplication       () { return _application;       }
		inline DocumentInterface* getDocumentInterface () { return _documentInterface; }

		inline EditArea*          getEditArea          () { return _editArea;          }
		inline StatusBar*         getStatusBar         () { return _status;            }
		inline StaffToolbar*      getStaffToolbar      () { return _staff;             }

		inline HWND               getWindowHandle      () { return _hWindow;           }
		inline HMENU              getMenu              () { return GetMenu(_hWindow);  }

	public:
	
		static const char APPLICATION_NAME[];
		// No description
		
	protected:

		static LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		       LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		void onCommand  (WPARAM , LPARAM );
		void onNotify   (WPARAM , LPARAM );

		void onCreate   (HWND );
		void onClose    ( );
		void onDestroy  ( );
		void onSize     ( );
		void onDropFiles(HDROP );

		void onEditSettingsFile(WORD );

	protected:

		static const char WINDOW_CLASS_NAME[];

		Application*       _application;

		DocumentInterface* _documentInterface;
		SettingsInterface* _settingsInterface;
		PrinterInterface*  _printerInterface;

		MainToolbar*       _toolbar;
		StatusBar*         _status;
		ChordsTabControl*  _chordsTabControl;
		EditArea*          _editArea;
		StaffToolbar*      _staff;

		HWND               _hWindow;

		unsigned int       _findReplaceCommandId;

};


#endif // MAIN_WINDOW_H

