#include "ChordGroup.h"


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
	DynamicPointerArray<GuitarChord>::addElement(newChord);
}

