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

#ifndef EDITIONHEURISTICS_H
#define EDITIONHEURISTICS_H

#include "../../util/utilities.h"
#include "../../data/GuitarChord.h"
#include "../../data/TuningDefinitions.h"

/* forward */ class EditArea;


/**
 * A few tools to navigate inside a tablature (find staffs, find lines, etc.)
 */
class EditionToolkit
{

	public:

		// initialisation /////////////

		EditionToolkit(EditArea*);
		~EditionToolkit();

		void setWindowHandle(HWND );


		// Work on selection //////////

/// Creates a simpler version of a procedure, so that client does not have to specify which object to work on
#define DECLARE_SHORT_VERSION(ReturnType, SelectionProcedure, ReturnMode) \
		inline ReturnType SelectionProcedure() { DWORD selection = getSelection(); ReturnMode SelectionProcedure(selection); }

		bool isSelectionValid(DWORD );                DECLARE_SHORT_VERSION(bool, isSelectionValid, return);
		bool isTextSelected(DWORD );		          DECLARE_SHORT_VERSION(bool, isTextSelected, return);
		bool doesSelectionEndLine(DWORD );            DECLARE_SHORT_VERSION(bool, doesSelectionEndLine, return);
		
		bool isStaffLine(DWORD );                     DECLARE_SHORT_VERSION(bool, isStaffLine, return);
		bool isStaffLine(unsigned int );
		bool isInsideStaff(DWORD );                   DECLARE_SHORT_VERSION(bool, isInsideStaff, return);
		unsigned int getStaffFirstLineIndex(DWORD );  DECLARE_SHORT_VERSION(unsigned int, getStaffFirstLineIndex, return);
		unsigned int getStaffLastLineIndex(DWORD );   DECLARE_SHORT_VERSION(unsigned int, getStaffLastLineIndex, return);

		void moveToNextLine(DWORD& );                 DECLARE_SHORT_VERSION(void, moveToNextLine, {} );
		void moveToStaffStart(DWORD& );               DECLARE_SHORT_VERSION(void, moveToStaffStart, {} );


		// API wrapping functions /////

#define __startOf(selection) LOWORD(selection)
#define __endOf(selection) HIWORD(selection)

		unsigned int getLine(unsigned int line, char*& buffer) const;

		inline unsigned int getLineIndex     (unsigned int position)  const { return SendMessage(_hWindow, EM_LINEFROMCHAR, (WPARAM)position, 0); }
		inline unsigned int getLineLength    (unsigned int position)  const { return SendMessage(_hWindow, EM_LINELENGTH,   (WPARAM)position, 0); }
		inline unsigned int getLineStart     (unsigned int line)      const { return SendMessage(_hWindow, EM_LINEINDEX,    (WPARAM)line,     0); }
		inline unsigned int getLineCount     ( )                      const { return SendMessage(_hWindow, EM_GETLINECOUNT, 0,                0); }

		       unsigned int getCharacterIndex (unsigned int& , unsigned int& );
		inline unsigned int getColumnIndex    (unsigned int position)         const { return position - getLineStart( getLineIndex(position) ); }

		inline DWORD getSelection     ( )                       const { return SendMessage(_hWindow, EM_GETSEL,       0,                0); }
		inline void  setSelection (unsigned int start, unsigned int end) { SendMessage(_hWindow, EM_SETSEL, (WPARAM)start, (LPARAM)end); }
		inline void  setSelection     (DWORD selection)               { setSelection(__startOf(selection), __endOf(selection)); }
		inline void  replaceSelection (const char* string ="")        { SendMessage(_hWindow, EM_REPLACESEL,  TRUE, (LPARAM)string); }

		       bool  selectNextOccurence(LPFINDREPLACE );


		// Other utilities ////////////

		inline EditArea* getEditArea() const { return _editArea; }


		void copyNoteAtBufferStart (GuitarChord* , unsigned int , char* );
		void copyAndFillAtLineCol  (const char* , unsigned int , unsigned int , unsigned int , char ='-');

		void  saveCursorPosition    (DWORD );          DECLARE_SHORT_VERSION(void, saveCursorPosition, {} );
		void  restoreCursorPosition (int =0, int =0);

	private:

		EditArea* _editArea;
		HWND      _hWindow;
		POINT     _cursorPosition;
		
};


#endif // EDITIONHEURISTICS_H

