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


#endif // DYNAMICPOINTERARRAY_CPP

