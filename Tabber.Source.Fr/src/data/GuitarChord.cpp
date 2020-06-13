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

#include "GuitarChord.h"

const char GuitarChord::EXTENDED_SEQUENCE_SEPARATOR = ',';
const char GuitarChord::INVISIBLE_NOTE = 'x';


/**
 * Simple constructor for chords.
 * Chords can be created in two ways, by passing either a 'simple' note sequence
 * e.g. 022100) for chords whose notes have only one character, or an 'extended'
 * note sequence for chords whose notes have multiple characters
 * (e.g. 10,12,12,11,10,10)
 */
GuitarChord::GuitarChord(const char* chordName, const char* chordNotes)
{
	_name = new char[lstrlen(chordName)+1]; //+1 for the final
	lstrcpy(_name, chordName);
	
	//try to handle chordNotes as a simple sequence
	int length = lstrlen(chordNotes);
	_isExtended = false;
	_noteCount = length;
	_width = 1;
	_notes = new char*[_noteCount];

	for(int pos=0; pos<length && !_isExtended; ++pos)
	{
		_notes[pos] = new char[2];
		
		if(chordNotes[pos] == EXTENDED_SEQUENCE_SEPARATOR)
		{
			// I've found a separator, go to 'extended mode'
			for(int index=pos; index>-1; --index)
			{
				delete [] _notes[index];
			}
			delete [] _notes;
			_isExtended = true;
			createExtendedChord(chordNotes);
		}
		else
		{
			_notes[pos][0] = chordNotes[pos];
			_notes[pos][1] = '\0';
		}
	}

	OBJECT_CREATED;
}


/**
 * Extended constructor for chords.
 * @warning may only be used after simple constructor, if this one has failed
 */
void GuitarChord::createExtendedChord(const char* chordNotes)
{
	// I assume classic constructor did already initialise _name
	int length = lstrlen(chordNotes);
	_isExtended = true;

	// first pass: count notes (required for memory allocation)
	_noteCount = 0;
	for(int pos=0; pos<length; ++pos)
	{
		if(chordNotes[pos] == EXTENDED_SEQUENCE_SEPARATOR)
		{
			++_noteCount;
		}
	}
	++_noteCount; // notesCount = separatorCount + 1

	_notes = new char*[_noteCount];

	// second pass: read
	char buffer[10];
	int bufferIndex = 0;
	int bufferLength = 0;
	int currentNoteIndex = 0;

	for(int index=0; index<length; ++index)
	{
		if(chordNotes[index] == EXTENDED_SEQUENCE_SEPARATOR)
		{
			buffer[bufferIndex] = '\0';
			_notes[currentNoteIndex] = new char[bufferLength+1];
			lstrcpy(_notes[currentNoteIndex], buffer);

			_width = max( _width, bufferLength );

			bufferIndex = 0;
			buffer[bufferIndex] = '\0';
			bufferLength = 0;
			++currentNoteIndex;
		}
		else
		{
			buffer[bufferIndex] = chordNotes[index];
			++bufferIndex;
			++bufferLength;
		}
	}

	//and the last one
	buffer[bufferIndex] = '\0';
	_notes[currentNoteIndex] = new char[lstrlen(buffer)+1];
	lstrcpy(_notes[currentNoteIndex], buffer);
}


GuitarChord::~GuitarChord()
{
	delete [] _name;

	for(int index=0; index<_noteCount; ++index)
	{
		delete [] _notes[index];
	}
	delete [] _notes;

	OBJECT_DELETED;
}


/**
 * Retrieves the full note sequence.
 */
const char* GuitarChord::getNotes() const
{
	conversionBuffer[0] = '\0';

	for(int n=0; n<_noteCount; n++)
	{
		lstrcat(conversionBuffer, _notes[n]);
		if(_isExtended) lstrcat(conversionBuffer, ","); // #('"' EXTENDED_SEQUENCE_SEPARATOR '"')
	}

	//remove the one too many separator
	conversionBuffer[lstrlen(conversionBuffer)-1] = '\0';

	return (const char*)conversionBuffer;
}


/// conversion buffer used for all chord instances
char GuitarChord::conversionBuffer[40];

