#ifndef EDITIONHEURISTICS_H
#define EDITIONHEURISTICS_H

#include "../util/utilities.h"

/* forward */ class EditArea;


/**
 * A few heuristics to navigate inside a tablature (find staffs, find lines, etc.)
 */
class EditionHeuristics
{

	public:

		EditionHeuristics(EditArea*);
		~EditionHeuristics();

		void setWindowHandle(HWND );

/// Creates a simpler version of a procedure, so that client does not have to specify which object to work on
#define DECLARE_SHORT_VERSION(ReturnType, SelectionProcedure) \
		inline ReturnType SelectionProcedure() { DWORD selection = (DWORD)SendMessage(_hWindow, EM_GETSEL, 0, 0); return SelectionProcedure(selection); }
	
		bool isTextSelected(DWORD );			DECLARE_SHORT_VERSION(bool, isTextSelected);
		bool doesSelectionEndLine(DWORD );      DECLARE_SHORT_VERSION(bool, doesSelectionEndLine);
		
		bool isStaffLine(DWORD );               DECLARE_SHORT_VERSION(bool, isStaffLine);
		bool isStaffLine(unsigned int );
		bool isInsideStaff(DWORD );             DECLARE_SHORT_VERSION(bool, isInsideStaff);

		void moveToNextLine(DWORD& );           DECLARE_SHORT_VERSION(void, moveToNextLine);

	private:

		EditArea* _editArea;
		HWND _hWindow;
		
};

#endif // EDITIONHEURISTICS_H

