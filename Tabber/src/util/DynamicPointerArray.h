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

