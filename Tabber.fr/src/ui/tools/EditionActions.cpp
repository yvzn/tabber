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

#include "EditionActions.h"
#include "../../ui/EditArea.h"
#include "../../ui/MainWindow.h"


// Actions ////////////////////////////////////////////////////////////////////

StaffAction::~StaffAction()
{ }



// Dispatcher /////////////////////////////////////////////////////////////////

Dispatcher::~Dispatcher()
{ }


void Dispatcher::applyAt(unsigned int line, unsigned int column, EditionToolkit* toolkit, bool isHeader)
{
	if(isHeader)
	{
		toolkit->copyAndFillAtLineCol(getHeaderString(), line, column, getStringWidth(), ' ');
	}
	else
	{
		toolkit->copyAndFillAtLineCol(getString(line), line, column, getStringWidth());
	}
}



// SingleNoteDispatcher ////////////////////////////////////////////////////////

/**
 * @param line number of the string where note is dispatched. Use -1 for header
 */
SingleNoteDispatcher::SingleNoteDispatcher(const char* note, int line)
{
	_width = lstrlen(note);
	_string = new char[_width+1];
	lstrcpy(_string, note);
	_line = line;
	OBJECT_CREATED;
}

/**
 * Constructs a single note from a key code, as provided by Windows' WM_KEY* messages
 * @param line number of the string where note is dispatched. Use -1 for header
 */
SingleNoteDispatcher::SingleNoteDispatcher(int virtualKeyCode, int line)
{
	_width = 1;
	_string = new char[_width+1];
	_string[0] = (char)virtualKeyCode;
	_string[1] = '\0';
	_line = line;
	OBJECT_CREATED;
}


SingleNoteDispatcher::~SingleNoteDispatcher()
{
	delete [] _string;
	OBJECT_DELETED;
}



// MultipleNoteDispatcher ////////////////////////////////////////////////////////////

MultipleNoteDispatcher::MultipleNoteDispatcher(GuitarChord* chord)
{
	_chord = chord;
	_width = -1;
	_lineFirstCall = -1;
	OBJECT_CREATED;
}


MultipleNoteDispatcher::~MultipleNoteDispatcher()
{
	OBJECT_DELETED;
}


void MultipleNoteDispatcher::applyAt(unsigned int line, unsigned int column, EditionToolkit* toolkit, bool isHeader)
{
	if(_width<0) initStringWidth(toolkit); // first call initialises this, so that I can easily fetch string width on next calls

	Dispatcher::applyAt(line, column, toolkit, isHeader);
}


const char* MultipleNoteDispatcher::getString(int line) 
{
	if(_lineFirstCall<0) _lineFirstCall = line; // first call initialises this, so that I can easily fetch string # on next calls

	int string = line - _lineFirstCall;
	return getNote( _chord->getNoteCount() - string - 1 ); // notes order is reversed :/
}



// ChordDispatcher ////////////////////////////////////////////////////////////

ChordDispatcher::ChordDispatcher(GuitarChord* chord)
	: MultipleNoteDispatcher(chord)
{ }


void ChordDispatcher::initStringWidth(EditionToolkit* toolkit)
{
	ApplicationSettings* settings =  toolkit->getEditArea()->getMainWindow()->getApplication()->getSettings();
	int nameWidth = lstrlen( _chord->getName() );
	_width = settings->isChordModeEnabled(ADD_NAME) ? max ( _chord->getWidth(), nameWidth ) : _chord->getWidth();
	if(settings->isChordModeEnabled(ADD_EXTRA_SPACE)) ++_width;
}


const char* ChordDispatcher::getNote(int string)
{
	if(-1 < string && string < _chord->getNoteCount())
	{
		return _chord->getNote(string);
	}
	else
	{
		return NULL;
	}
}



// TuningDispatcher ///////////////////////////////////////////////////////////

TuningDispatcher::TuningDispatcher(GuitarTuning* tuning)
	: MultipleNoteDispatcher(tuning)
{
	// tuning is stored as a chord in SuperClass

	_buffer = new char[_chord->getWidth() + 1 + 1]; // +1='|' +1='\0'
	_buffer[_chord->getWidth()] = '|';
	_buffer[_chord->getWidth()+1] = '\0';

	OBJECT_CREATED;
}


TuningDispatcher::~TuningDispatcher()
{
	delete [] _buffer;
	OBJECT_DELETED;
}


void TuningDispatcher::applyAt(unsigned int line, unsigned int column, EditionToolkit* toolkit, bool isHeader)
{
	MultipleNoteDispatcher::applyAt(line, 0, toolkit, isHeader); // tunings are always on line start
}


const char* TuningDispatcher::getNote(int string)
{
	for(int prefix=0; prefix<_chord->getWidth(); ++prefix) _buffer[prefix]=' ';

	if(-1 < string && string < _chord->getNoteCount())
	{
		const char* note = _chord->getNote(string);
		CopyMemory(_buffer, note, lstrlen(note)); // does not copy the final '\0'
	}
	
	return _buffer;
}



// ArpeggioDispatcher /////////////////////////////////////////////////////////

ArpeggioDispatcher::ArpeggioDispatcher(GuitarChord* chord, Direction dir)
	: MultipleNoteDispatcher(chord)
{
	_buffer = NULL;
	_direction = dir;
	OBJECT_CREATED;
}


ArpeggioDispatcher::~ArpeggioDispatcher()
{
	if(_buffer != NULL) delete [] _buffer;
	OBJECT_DELETED;
}


void ArpeggioDispatcher::initStringWidth (EditionToolkit* toolkit)
{
	_width = 0;
	_addSpace = toolkit->getEditArea()->getMainWindow()->getApplication()->getSettings()->isChordModeEnabled(ADD_EXTRA_SPACE);

	for(int string=0; string < _chord->getNoteCount(); ++string)
	{
		const char* note = _chord->getNote(string);
		if(note != NULL)
		{
			_width += lstrlen(note);
			if(_addSpace) ++_width;
		}
	}
}


const char* ArpeggioDispatcher::getNote(int string)
{
	assert(_width >= 0); //e.g. initStringWidth called somewhere before

	if(_buffer == NULL) _buffer = new char[_width+1];

	unsigned int pos=0;

	int start, end, incr;
	if(_direction==ASCENDING)       { start=0;    end=_chord->getNoteCount(); incr=+1; }
	else /*_direction==DESCENDING*/ { start=_chord->getNoteCount()-1; end=-1; incr=-1; }

	for(int index=start;
		(_direction == ASCENDING) && (index < end) || (_direction != ASCENDING) && (index > end);
		index+=incr)
	{
		const char* note = _chord->getNote(index);
		if(note != NULL)
		{
			int length = lstrlen(note);
			if(index == string)
			{
				CopyMemory(&_buffer[pos], note, length);
				pos += length;

				if(_addSpace)
				{
					_buffer[pos] = '-';
					pos++;
				}
			}
			else
			{
				int paddingSize = length + (_addSpace ? 1 : 0);
				for(int padding=0; padding<paddingSize; ++padding)
				{
					_buffer[pos+padding] = '-';
				}
				pos += paddingSize;
			}
		}
	}

	_buffer[pos] = '\0';
	return _buffer;
}



// Remover ////////////////////////////////////////////////////////////////////

void Remover::applyAt(unsigned int line, unsigned int column, EditionToolkit* toolkit, bool isHeader)
{
	unsigned int newColumn = column;
	unsigned int position = toolkit->getCharacterIndex(line, newColumn);

	if( column == newColumn
		&& (_direction <=0
		|| (_direction > 0 && column < toolkit->getLineLength(position))) )
	{
		toolkit->setSelection(position, position + _direction);
		toolkit->replaceSelection("");
	}
}
