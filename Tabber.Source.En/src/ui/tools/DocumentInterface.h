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

#ifndef DOCUMENTINTERFACE_H
#define DOCUMENTINTERFACE_H

#include "../../util/utilities.h"

/* forward */ class MainWindow;


/**
 * Storage class for document state
 */
class DocumentInterface
{

	public:

		DocumentInterface(MainWindow*);
		~DocumentInterface();

		void setDocumentModified(bool);
		bool continueIfDocumentModified();
		
		void onNewDocument    ( );
		bool onDocumentSave   ( );
		bool onDocumentSaveAs ( );
		bool onDocumentOpen   ( );
		bool onDocumentOpen   (const char* );

		void onFind    ( );
		void onFindNext( );
		void onReplace ( );

		inline const char* getFileName() const { return _fileName; }

		inline bool interceptsMessage(MSG* message) { return TRUE == IsDialogMessage(_hFindReplaceDialog, message); }

	protected:

		void blankDocumentFlags();
		void updateMainWindowTitle();
		void updateFileName();
		bool loadSpecifiedDocument();

		static UINT APIENTRY handleFindReplaceDialogMessage(HWND , UINT , WPARAM , LPARAM );

	protected:

		MainWindow* _mainWindow;

		OPENFILENAME _fileDialogOptions;

		bool  _isDocumentModified;
		bool  _isFileLoaded;
		char  _filePathAndName[MAX_PATH];
		char* _fileName;

		FINDREPLACE _findReplaceOptions;

		HWND  _hFindReplaceDialog;
		static bool _isDialogOpened;
		char* _findWhat;
		char* _replaceWith;

};

#endif // DOCUMENTINTERFACE_H

