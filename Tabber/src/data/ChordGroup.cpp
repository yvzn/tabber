#include "ChordGroup.h"


CChordGroup::CChordGroup(LPCSTR szGroupName)
{
	m_szName = (LPSTR)GlobalAlloc(GPTR, strlen(szGroupName));
	strcpy(m_szName, szGroupName);

	OBJECT_CREATED;
}


CChordGroup::~CChordGroup()
{
	GlobalFree(m_szName);
	OBJECT_DELETED;
}


LPSTR CChordGroup::getName()
{
	return m_szName;
}


int CChordGroup::getElementCount()
{
	return m_nElementCount;
}


void CChordGroup::setElementCount(int nNewCount)
{
	m_nElementCount = nNewCount;
}

