#ifndef CHORD_H
#define CHORD_H

#include "../core/utilities.h"

#define CHORD_DEPTH 6


class CChord
{

	public:

		CChord(LPCSTR , LPCSTR );
		virtual ~CChord();
		
		LPSTR getName  (); 
		LPSTR getNotes ();
		char  getNote  (int ); 
		
	protected:

		LPSTR m_szName;
		char  m_cNotes[CHORD_DEPTH];

};

#endif // CHORD_H

