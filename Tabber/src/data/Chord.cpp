#include "Chord.h"


CChord::CChord(LPCSTR szChordName, LPCSTR szChordNotes)
{
	m_szName = (LPSTR)GlobalAlloc(GPTR, strlen(szChordName));
	strcpy(m_szName, szChordName);
	
	int nNotesCount = strlen(szChordNotes);
	int i=0;
	for( ; i<CHORD_DEPTH && i<nNotesCount; i++)
	{
		m_cNotes[i] = szChordNotes[i];
	}
	for( ; i<CHORD_DEPTH; i++)
	{
		m_cNotes[i] = 'x';
	}
	
	OBJECT_CREATED;
}

CChord::~CChord()
{
	GlobalFree(m_szName);
	OBJECT_DELETED;
}


LPSTR CChord::getName()
{
	return m_szName;
}


LPSTR CChord::getNotes()
{
	LPSTR szNotes;
  	szNotes = (LPSTR)GlobalAlloc(GPTR, CHORD_DEPTH+1);
	lstrcpy(szNotes, m_cNotes);
  	szNotes[CHORD_DEPTH] = '\0';
	return szNotes;
}


char CChord::getNote(int nString)
{
	return m_cNotes[nString];
}


