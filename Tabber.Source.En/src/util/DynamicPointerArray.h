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

#ifndef DYNAMICPOINTERARRAY_H
#define DYNAMICPOINTERARRAY_H

#include "../util/utilities.h"


template <class Type>
class DynamicPointerArray
{

	public:

		DynamicPointerArray();
		virtual ~DynamicPointerArray();

		virtual void  addElement(Type* );

		inline virtual Type* getElementAt    (int index)  { assert(index < _elementCount); return _values[index]; }
		inline virtual int   getElementCount ()     const { return _elementCount; }

	protected:

		int    _arraySize;
		int    _elementCount;
		Type** _values;

};


// .cpp file is included here because of template instantiation bugs
#include "DynamicPointerArray.cpp"


#endif // DYNAMICPOINTERARRAY_H

