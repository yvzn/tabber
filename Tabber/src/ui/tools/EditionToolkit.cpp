#include "EditionToolkit.h"
#include "../../ui/EditArea.h"
#include "../../ui/MainWindow.h"



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
	assert(_hWindow != NULL);

	return getLineIndex( __endOf(selection) ) < getLineCount();
}


/**
 * Says if some text is selected somewhere in the edit control
 */
bool EditionToolkit::isTextSelected(DWORD selection)
{
	return __endOf(selection) != __startOf(selection);
}


/**
 * Says if selection ends at the end of the line
 */
bool EditionToolkit::doesSelectionEndLine(DWORD selection)
{
	assert(_hWindow != NULL);

	WORD selectionEnd = __endOf(selection);
	int  lineIndex    = getLineIndex(selectionEnd);

	return (selectionEnd - getLineStart(lineIndex) == getLineLength(selectionEnd));
}


/**
 * Says if the line containing current selection can be considered in a staff line
 * Heuristic: Any line with more than 5 occurences of '-' is potentially accepted
 */
bool EditionToolkit::isStaffLine(DWORD selection)
{
    assert(_hWindow != NULL);
	return isStaffLine( getLineIndex( (WPARAM)__endOf(selection) ) );
}


/**
 * Says if specified line can be considered as part of a staff
 */
bool EditionToolkit::isStaffLine(unsigned int lineIndex)
{
    assert(_hWindow != NULL);

    int lineStart  = getLineStart (lineIndex);
	int lineLength = getLineLength(lineStart);
	WORD bufferSize = lineLength+2;

	char* buffer = new char[bufferSize];
	CopyMemory(buffer, &bufferSize, sizeof(WORD)); // see EM_GETLINE reference
	SendMessage(_hWindow, EM_GETLINE, (WPARAM)lineIndex, (LPARAM)buffer);

	int occurences=0;
	for(int index=0; index<lineLength; ++index)
	{
		if(buffer[index] == '-') ++occurences;
	}

	delete [] buffer;
	return occurences > 5;
}


unsigned int EditionToolkit::getStaffFirstLineIndex(DWORD selection)
{
	assert(_hWindow != NULL && isStaffLine(selection));

	int lineIndex = getLineIndex( __endOf(selection) );
	while(lineIndex>=0 && isStaffLine((unsigned int)lineIndex))
	{
		--lineIndex;
	}
	++lineIndex; //loop goes one line too far
	return lineIndex;
}


unsigned int EditionToolkit::getStaffLastLineIndex(DWORD selection)
{
	assert(_hWindow != NULL && isStaffLine(selection));

	unsigned int lineIndex = getLineIndex( __endOf(selection) );
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
 */
bool EditionToolkit::isInsideStaff(DWORD selection)
{
	assert(_hWindow != NULL);

	unsigned int currentLine = getLineIndex( __endOf(selection) );
	unsigned int chordDepth = _editArea->getMainWindow()->getApplication()->getSettings()->getChordDepth();

	return isStaffLine(currentLine)
		&& getStaffLastLineIndex(selection) - getStaffFirstLineIndex(selection) + 1 >= chordDepth;
}


/**
 * Moves selection to the beginning of next line.
 * If selection is inside a staff, move to the beginning of the first line following the staff.
 * If selection is a the end of the file, appends a new blank line and move to its beginning.
 * @param selection in/out, location of current selection, recieves new location of selection.
 */
void EditionToolkit::moveToNextLine(DWORD& selection)
{
	assert(_hWindow != NULL);

	unsigned int selEnd = __endOf(selection);
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
 * @param selection in/out, location of current selection, recieves new location of selection.
 */
void EditionToolkit::moveToStaffStart(DWORD& selection)
{
	assert(_hWindow != NULL && isStaffLine(selection));

	unsigned int firstLineIndex  = getStaffFirstLineIndex(selection);

	// move to line start
	unsigned int lineStart = getLineStart(firstLineIndex);
	setSelection(lineStart, lineStart);
	selection = lineStart << 16 | lineStart;
}



// OTHER UTILITIES ////////////////////////////////////////////////////////////


/**
 * Safe conversion from (line, column) to character index (as used in edit controls)
 * If line is not long enough to reach column, the last column of the line is used.
 * If line is greater than total number of lines, the last line is used.
 */
unsigned int EditionToolkit::getCharacterIndex(unsigned int& line, unsigned int& column)
{
	assert(_hWindow != NULL);

	unsigned int b; // used to save multiple calls in min macro

	b = getLineCount() - 1;
	line = min ( line , b );
	unsigned int lineStart = getLineStart(line);

	b = getLineLength ( lineStart );
	column = min ( column , b );

	return lineStart + column;
}


/**
 * Copies the nth note of a chord at the beginning of a buffer, adding space if required and removing unnecessary '\0's
 */
void  EditionToolkit::copyNoteAtBufferStart(GuitarChord* chord, unsigned int noteIndex, char* buffer)
{
	for(int prefix=0; prefix<chord->getWidth(); ++prefix) buffer[prefix]=' ';

	if(noteIndex < chord->getNoteCount())
	{
		const char* note = chord->getNote(chord->getNoteCount() - noteIndex - 1); // note order is reversed :/
		CopyMemory(buffer, note, lstrlen(note)); // does not copy the final '\0'
	}

	buffer[chord->getWidth()] = '|';
}


/**
 * Copies at least (requiredWidth) characters from buffer to edit control, at specified position.
 * If the buffer is smaller than (requiredWidth), extra (paddingCharacter)s are added.
 * If line is not long enough to reach column, the buffer is copied at the end of the line.
 * If line is greater than total number of lines, nothing is done.
 */
void EditionToolkit::copyAndFillAtLineCol(
	const char*  buffer,
	unsigned int line,
	unsigned int col,
	unsigned int requiredWidth,
	char         paddingCharacter )
{
	assert(_hWindow != NULL);

	if(line < getLineCount())
	{
		unsigned int position = getCharacterIndex(line, col);
		setSelection(position, position);

		unsigned int length = lstrlen(buffer);
  		if(length >= requiredWidth)
		{
			replaceSelection(buffer);
		}
		else
		{
			char* fullString = new char[requiredWidth+1];

			CopyMemory(fullString, buffer, length);
			for(unsigned int pos=length; pos<requiredWidth; ++pos)
			{
				fullString[pos] = paddingCharacter;
			}
			fullString[requiredWidth] = '\0';

			replaceSelection(fullString);
			delete [] fullString;
		}
	}
}


void EditionToolkit::saveCursorPosition(DWORD selection)
{
	assert(_hWindow != NULL);

	// Just saving selection location is not enough because selection location
	// is based on character-indexes, and if characters are typed between
	// save and restore, character-indexes are changed.
	// I prefer to rely on line/column location.

	WORD selectionEnd = __endOf( selection );
	_cursorPosition.x = getColumnIndex( selectionEnd );
	_cursorPosition.y = getLineIndex( selectionEnd );
}


/**
 * @param columnOffset extra offset added to cursor position, might be useful e.g. when restoring cursor position after typing
 * @param lineOffset   extra offset added to cursor position, might be useful e.g. when restoring cursor position after typing
 */
void EditionToolkit::restoreCursorPosition(int columnOffset, int lineOffset)
{
	assert(_hWindow != NULL);
	unsigned int line = _cursorPosition.y + lineOffset;
	unsigned int col = _cursorPosition.x + columnOffset;
	unsigned int position = getCharacterIndex( line , col );
	setSelection(position, position);
}
