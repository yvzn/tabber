#include "DynamicArray.h"

#ifndef DYNAMICARRAY_CPP
#define DYNAMICARRAY_CPP


template <class Type>
CDynamicArray<Type>::CDynamicArray()
{
	m_nArraySize = 0;
	m_nElementCount = 0;
	m_lpValues = NULL;
	
	OBJECT_CREATED;
}


template <class Type>
CDynamicArray<Type>::~CDynamicArray()
{
	for(int nIndex=0; nIndex<m_nElementCount; nIndex++)
	{
		delete m_lpValues[nIndex];
	}
	
	GlobalFree(m_lpValues);
	OBJECT_DELETED;
}


template <class Type>
void CDynamicArray<Type>::addElement(Type* lpNewElement)
{
	Type** lpTemp;
	
	if(m_nElementCount >= m_nArraySize)
	{
		if(m_nArraySize == 0)
		{
			m_nArraySize = 1;
		}
		else
		{
			m_nArraySize *= 2;
		}
		
		lpTemp = (Type**)GlobalAlloc(GPTR, m_nArraySize * sizeof(Type*));
		CopyMemory(lpTemp, m_lpValues, m_nElementCount * sizeof(Type*));
		GlobalFree(m_lpValues);
  		m_lpValues = lpTemp;		
	}
	m_lpValues[m_nElementCount] = lpNewElement;
	++m_nElementCount;
}


template <class Type>
Type* CDynamicArray<Type>::getElementAt(int nIndex)
{
	assert(nIndex < m_nElementCount);
	return m_lpValues[nIndex];
}


template <class Type>
int CDynamicArray<Type>::getElementCount()
{
	return m_nElementCount;
}


#endif // DYNAMICARRAY_CPP

