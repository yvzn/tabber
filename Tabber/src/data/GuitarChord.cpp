#include "GuitarChord.h"


GuitarChord::GuitarChord(const char* chordName, const char* chordNotes)
{
	int chordNoteCount = lstrlen(chordNotes);

	_name = new char[lstrlen(chordName)+1];
	lstrcpy(_name, chordName);
	
	_notes = new char[CHORD_DEPTH+1];
	CopyMemory(_notes, chordNotes, chordNoteCount);
	
	//fill the remaining space
	for(int i=chordNoteCount; i<CHORD_DEPTH; i++) {	_notes[i] = 'x'; }
	_notes[CHORD_DEPTH] = '\0';

	OBJECT_CREATED;
}


GuitarChord::~GuitarChord()
{
	delete [] _name;
	delete [] _notes;
	OBJECT_DELETED;
}


const char* GuitarChord::getName() const
{
	assert(_name != NULL);
	return _name;
}


const char* GuitarChord::getNotes() const
{
	assert(_notes != NULL);
	return _notes;
}


char GuitarChord::getNote(int string) const
{
	assert(_notes != NULL);
	return _notes[string];
}


