#ifndef DYNAMICPOINTERARRAY_H
#define DYNAMICPOINTERARRAY_H

#include "../util/utilities.h"


template <class Type>
class DynamicPointerArray
{

	public:

		DynamicPointerArray();
		virtual ~DynamicPointerArray();
		
		virtual void  addElement      (Type* )      ;
		virtual Type* getElementAt    (int )        ;
		virtual int   getElementCount ()       const;
		
	protected:
	
		int    _arraySize;
		int    _elementCount;
		Type** _values;

};


// .cpp file is included here because of template instantiation bugs
#include "DynamicPointerArray.cpp"


#endif // DYNAMICPOINTERARRAY_H

