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


const char* ChordGroup::getName() const
{
	assert(_name != NULL);
	return _name;
}


int ChordGroup::getChordCount() const
{
	return DynamicPointerArray<GuitarChord>::getElementCount();
}


void ChordGroup::addChord(GuitarChord* newChord)
{
	DynamicPointerArray<GuitarChord>::addElement(newChord);
}


GuitarChord* ChordGroup::getChordAt (int index)
{
	return DynamicPointerArray<GuitarChord>::getElementAt(index);
}

