#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "../core/utilities.h"


template <class Type>
class CDynamicArray
{

	public:

		CDynamicArray();
		~CDynamicArray();
		
		void  addElement      (Type* );
		Type* getElementAt    (int );
		int   getElementCount ();
		
	protected:
	
		int    m_nArraySize;
		int    m_nElementCount;
		Type** m_lpValues;

};


// .cpp file is included here because of template instantiation bugs
#include "DynamicArray.cpp"


#endif // DYNAMICARRAY_H

