#include "ChordGroup.h"


ChordGroup::ChordGroup(const char* groupName, int elementCount)
{
	_name = new char[lstrlen(groupName)+1];
	strcpy(_name, groupName);

	_elementCount = elementCount;

	OBJECT_CREATED;
}


ChordGroup::~ChordGroup()
{
	delete [] _name;
	OBJECT_DELETED;
}


const char* ChordGroup::getName() const
{
	assert(_name != NULL);
	return _name;
}


int ChordGroup::getElementCount() const
{
	return _elementCount;
}


void ChordGroup::setElementCount(int newCount)
{
	_elementCount = newCount;
}

