#include "EditionHeuristics.h"
#include "../ui/EditArea.h"
#include "../ui/MainWindow.h"


/**
 * @param editArea a pointer to the corrersponding edit area (so that heuristics can access its parameters)
 */
EditionHeuristics::EditionHeuristics(EditArea* editArea)
{
    _editArea = editArea;
	OBJECT_CREATED;
}

EditionHeuristics::~EditionHeuristics()
{
	OBJECT_DELETED;
}

/**
 * Tell the heuristics wich edit control they will be working on
 * @param hEdit a window handle to the edit control to monitor
 */
void EditionHeuristics::setWindowHandle(HWND hEdit)
{
	_hWindow = hEdit;
}


/**
 * Says if some text is selected somewhere in the edit control
 * @param selection MAKEWORD(selectionStart, selectionEnd)
 */
bool EditionHeuristics::isTextSelected(DWORD selection)
{
	return HIWORD(selection) != LOWORD(selection);
}


/**
 * Says if selection ends at the end of the line
 * @param selection MAKEWORD(selectionStart, selectionEnd)
 */
bool EditionHeuristics::doesSelectionEndLine(DWORD selection)
{
	assert(_hWindow != NULL);
	
	WORD selEnd = HIWORD(selection);
	
	int lineIndex  = SendMessage(_hWindow, EM_LINEFROMCHAR, selEnd, 0);
	int lineStart  = SendMessage(_hWindow, EM_LINEINDEX, lineIndex, 0);
	int lineLength = SendMessage(_hWindow, EM_LINELENGTH, selEnd, 0);

	return (selEnd - lineStart == lineLength);
}


/**
 * Says if the line containing current selection can be considered in a staff line
 * Heuristic: Any line with more than 5 occurences of '-' is potentially accepted
 * @param selection MAKEWORD(selectionStart, selectionEnd)
 */
bool EditionHeuristics::isStaffLine(DWORD selection)
{
    assert(_hWindow != NULL);
    
	unsigned int lineIndex = SendMessage(_hWindow, EM_LINEFROMCHAR, (WPARAM)HIWORD(selection), 0);
	return isStaffLine(lineIndex);
}


/**
 * Says if specified line can be considered as part of a staff
 */
bool EditionHeuristics::isStaffLine(unsigned int lineIndex)
{
    assert(_hWindow != NULL);
    
    int lineStart = SendMessage(_hWindow, EM_LINEINDEX, (WPARAM)lineIndex, 0);
	int lineLength = SendMessage(_hWindow, EM_LINELENGTH, (WPARAM)lineStart, 0);

	char* buffer = new char[lineLength+1];
	ZeroMemory(buffer, sizeof(buffer));
	buffer[0] = (char)lineLength+1; // see EM_GETLINE reference
	SendMessage(_hWindow, EM_GETLINE, (WPARAM)lineIndex, (LPARAM)buffer);

	int occurences=0;
	for(int index=0; index<lineLength; ++index)
	{
		if(buffer[index] == '-') ++occurences;
	}

	delete [] buffer;
	return occurences > 5;
}


/**
 * Says if current selection can be considered as inside a staff
 * Heuristic: Any character inside a group of 6 (CHORD_DEPTH) consecutive staff lines (as defined by isStaffLine) is accepted
 * @param selection MAKEWORD(selectionStart, selectionEnd)
 */
bool EditionHeuristics::isInsideStaff(DWORD selection)
{
	assert(_hWindow != NULL);

	unsigned int currentLine = SendMessage(_hWindow, EM_LINEFROMCHAR, (WPARAM)HIWORD(selection), 0);
	int lineCount = SendMessage(_hWindow, EM_GETLINECOUNT, 0, 0);
	int chordDepth = _editArea->getMainWindow()->getApplication()->getSettings()->getChordDepth();
	
	if(isStaffLine(currentLine))
	{
		int staffLinesInArow = 1;
		
		int lineAbove=1;
		while(
	        staffLinesInArow < chordDepth
			&& lineAbove < chordDepth
	        && (int)currentLine - lineAbove > -1
	        && isStaffLine((unsigned int)currentLine - lineAbove) )
				{
					++lineAbove;
		        	++staffLinesInArow;
				}

		int lineBelow=1;
		while(
  			staffLinesInArow < chordDepth
     		&& lineBelow < chordDepth
       		&& currentLine + lineBelow < lineCount
       		&& isStaffLine(currentLine + lineBelow) )
				{
					++lineBelow;
					++staffLinesInArow;
				}

		return staffLinesInArow >= chordDepth;
	}
	else
	{
		return false;
	}
}


/**
 * Moves selection to the beginning of next line.
 * If selection is inside a staff, move to the beginning of the first line following the staff.
 * If selection is a the end of the file, appends a new blank line and move to its beginning.
 * @param selection MAKEWORD(selectionStart, selectionEnd), in/out location of current selection, recieves new location of selection.
 */
void EditionHeuristics::moveToNextLine(DWORD& selection)
{
	assert(_hWindow != NULL);

	unsigned int selEnd = HIWORD(selection);
	unsigned int lineIndex  = SendMessage(_hWindow, EM_LINEFROMCHAR, selEnd, 0);
	unsigned int lineStart  = SendMessage(_hWindow, EM_LINEINDEX, lineIndex, 0);

	if(selEnd != lineStart)
	{
		if(isStaffLine(lineIndex))
		{
			unsigned int lineCount = SendMessage(_hWindow, EM_GETLINECOUNT, 0, 0);
			++lineIndex;
			while(isStaffLine(lineIndex) && lineIndex<lineCount)
			{
				++lineIndex;
			}

			//return to the last line inside staff
   			--lineIndex;
			lineStart = SendMessage(_hWindow, EM_LINEINDEX, lineIndex, 0);
			selEnd = lineStart;
		}

		unsigned int lineLength = SendMessage(_hWindow, EM_LINELENGTH, selEnd, 0);
		selEnd = lineStart + lineLength;

		//if we're at the end of file, adds a new line
		if(selEnd == SendMessage(_hWindow, WM_GETTEXTLENGTH, 0, 0))
		{
			SendMessage(_hWindow, EM_SETSEL, selEnd, selEnd);
			SendMessage(_hWindow, EM_REPLACESEL, TRUE, (LPARAM)"\r\n");
		}

		//goto next line (\r => +1, \n => +1)
		selEnd += 2;
	}

	SendMessage(_hWindow, EM_SETSEL, selEnd, selEnd);
	selection = MAKEWORD(selEnd, selEnd);
}

