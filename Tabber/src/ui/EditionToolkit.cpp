#include "EditionToolkit.h"
#include "../ui/EditArea.h"
#include "../ui/MainWindow.h"



// CONSTR / DESTR and initialisation //////////////////////////////////////////

/**
 * @param editArea a pointer to the corrersponding edit area (so that heuristics can access its parameters)
 */
EditionToolkit::EditionToolkit(EditArea* editArea)
{
    _editArea = editArea;
	OBJECT_CREATED;
}

EditionToolkit::~EditionToolkit()
{
	OBJECT_DELETED;
}

/**
 * Tell the heuristics wich edit control they will be working on
 * @param hEdit a window handle to the edit control to monitor
 */
void EditionToolkit::setWindowHandle(HWND hEdit)
{
	_hWindow = hEdit;
}



// ON SELECTION WORK /////////////////////////////////////////////////////////


/**
 * Says if selection can be considered as valid.
 * From time to time, initialisation chlores may result in passing invalid values as selections
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start
 * @return true if selection end line < getLineCount()
 */
bool EditionToolkit::isSelectionValid(DWORD selection)
{
	return getLineIndex(HIWORD(selection)) < getLineCount();
}


/**
 * Says if some text is selected somewhere in the edit control
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start
 */
bool EditionToolkit::isTextSelected(DWORD selection)
{
	return HIWORD(selection) != LOWORD(selection);
}


/**
 * Says if selection ends at the end of the line
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start
 */
bool EditionToolkit::doesSelectionEndLine(DWORD selection)
{
	assert(_hWindow != NULL);

	WORD selectionEnd = HIWORD(selection);
	int  lineIndex    = getLineIndex(selectionEnd);

	return (selectionEnd - getLineStart(lineIndex) == getLineLength(selectionEnd));
}


/**
 * Says if the line containing current selection can be considered in a staff line
 * Heuristic: Any line with more than 5 occurences of '-' is potentially accepted
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start
 */
bool EditionToolkit::isStaffLine(DWORD selection)
{
    assert(_hWindow != NULL);
	return isStaffLine( getLineIndex( (WPARAM)HIWORD(selection) ) );
}


/**
 * Says if specified line can be considered as part of a staff
 */
bool EditionToolkit::isStaffLine(unsigned int lineIndex)
{
    assert(_hWindow != NULL);
 
    int lineStart  = getLineStart (lineIndex);
	int lineLength = getLineLength(lineStart);

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
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start
 */
unsigned int EditionToolkit::getStaffFirstLineIndex(DWORD selection)
{
	//assert(isStaffLine(selection));

	int lineIndex = getLineIndex( HIWORD(selection) );
	while(lineIndex>=0 && isStaffLine((unsigned int)lineIndex))
	{
		--lineIndex;
	}
	++lineIndex; //loop goes one line too far
	return lineIndex;
}


/**
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start
 */
unsigned int EditionToolkit::getStaffLastLineIndex(DWORD selection)
{
	//assert(isStaffLine(selection));

	unsigned int lineIndex = getLineIndex( HIWORD(selection) );
	unsigned int lineCount = getLineCount();
	while(lineIndex<lineCount && isStaffLine(lineIndex))
	{
		++lineIndex;
	}
	--lineIndex; //loop goes one line too far
	return lineIndex;
}


/**
 * Says if current selection can be considered as inside a staff
 * Heuristic: Any character inside a group of 6 (CHORD_DEPTH) consecutive staff lines (as defined by isStaffLine) is accepted
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start
 */
bool EditionToolkit::isInsideStaff(DWORD selection)
{
	assert(_hWindow != NULL);

	unsigned int currentLine = getLineIndex(HIWORD(selection));
	unsigned int chordDepth = _editArea->getMainWindow()->getApplication()->getSettings()->getChordDepth();

	return isStaffLine(currentLine)
		&& getStaffLastLineIndex(selection) - getStaffFirstLineIndex(selection) + 1 >= chordDepth;
}


/**
 * Moves selection to the beginning of next line.
 * If selection is inside a staff, move to the beginning of the first line following the staff.
 * If selection is a the end of the file, appends a new blank line and move to its beginning.
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start, in/out, location of current selection, recieves new location of selection.
 */
void EditionToolkit::moveToNextLine(DWORD& selection)
{
	assert(_hWindow != NULL);

	unsigned int selEnd = HIWORD(selection);
	unsigned int lineIndex  = getLineIndex(selEnd);
	unsigned int lineStart  = getLineStart(lineIndex);

	if(selEnd != lineStart)
	{
		if(isStaffLine(lineIndex))
		{
			lineIndex = getStaffLastLineIndex(selection);
			lineStart = getLineStart(lineIndex);
			selEnd = lineStart;
		}

		unsigned int lineLength = getLineLength(selEnd);
		selEnd = lineStart + lineLength;

		//if we're at the end of file, adds a new line
		if(selEnd == (unsigned int)SendMessage(_hWindow, WM_GETTEXTLENGTH, 0, 0))
		{
			setSelection(selEnd, selEnd);
			replaceSelection("\r\n");
		}

		//goto next line (\r => +1, \n => +1)
		selEnd += 2;
	}

	setSelection(selEnd, selEnd);
	selection = selEnd << 16 | selEnd;
}


/**
 * Moves selection to the beginning of current staff.
 * @param selection HIWORD(selection) == end, LOWORD(selection) == start, in/out, location of current selection, recieves new location of selection.
 */
void EditionToolkit::moveToStaffStart(DWORD& selection)
{
	assert(isStaffLine(selection));

	unsigned int firstLineIndex  = getStaffFirstLineIndex(selection);

	// move to line start
	unsigned int lineStart = getLineStart(firstLineIndex);
	setSelection(lineStart, lineStart);
	selection = lineStart << 16 | lineStart;
}



// OTHER UTILITIES ////////////////////////////////////////////////////////////


/**
 * Copies the nth note of a chord at the beginning of a buffer, adding space if required and removing unnecessary '\0's
 */
void  EditionToolkit::copyNoteAtBufferStart(GuitarChord* chord, int noteIndex, char* buffer)
{
	for(int prefix=0; prefix<chord->getWidth(); ++prefix) buffer[prefix]=' ';

	if(noteIndex < chord->getNoteCount())
	{
		const char* note = chord->getNote(chord->getNoteCount() - noteIndex - 1); // note order is reversed :/
		lstrcpy(buffer, note);
		buffer[lstrlen(note)] = ' '; // removes the '\0' at the end of note
	}

	buffer[chord->getWidth()] = '|';
}



