#include "DynamicPointerArray.h"

#ifndef DYNAMICPOINTERARRAY_CPP
#define DYNAMICPOINTERARRAY_CPP


template <class Type>
DynamicPointerArray<Type>::DynamicPointerArray()
{
	_arraySize = 0;
	_elementCount = 0;
	_values = NULL;
	
	OBJECT_CREATED;
}


template <class Type>
DynamicPointerArray<Type>::~DynamicPointerArray()
{
	for(int index=0; index<_elementCount; index++)
	{
		delete _values[index];
	}
	
	delete [] _values;
	OBJECT_DELETED;
}


template <class Type>
void DynamicPointerArray<Type>::addElement(Type* newElement)
{
	Type** tempArray;
	
	if(_elementCount >= _arraySize)
	{
		_arraySize = _arraySize == 0 ? 1 : _arraySize *= 2;
		
		tempArray = new Type* [_arraySize];
		CopyMemory(tempArray, _values, _elementCount * sizeof(Type*));

  		delete [] _values;
  		_values = tempArray;		
	}
	_values[_elementCount] = newElement;
	++_elementCount;
}


template <class Type>
Type* DynamicPointerArray<Type>::getElementAt(int index)
{
	assert(index < _elementCount);
	return _values[index];
}


template <class Type>
int DynamicPointerArray<Type>::getElementCount() const
{
	return _elementCount;
}


#endif // DYNAMICPOINTERARRAY_CPP

