#include "ChordGroup.h"

const int  ChordGroup::MAX_CHORD_COUNT = 256;


ChordGroup::ChordGroup(const char* groupName)
{
	_name = new char[lstrlen(groupName)+1];
	strcpy(_name, groupName);

	OBJECT_CREATED;
}


ChordGroup::~ChordGroup()
{
    delete _name;
	OBJECT_DELETED;
}


void ChordGroup::addChord(GuitarChord* newChord)
{
	if( getChordCount() < MAX_CHORD_COUNT )
	{
		DynamicPointerArray<GuitarChord>::addElement(newChord);
	}
}

