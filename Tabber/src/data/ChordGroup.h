#ifndef CHORDGROUP_H
#define CHORDGROUP_H

#include "../core/utilities.h"


class CChordGroup
{

	public:

		CChordGroup(LPCSTR );
		virtual ~CChordGroup();

		LPSTR getName();
		int   getElementCount(); 
		void  setElementCount(int ); 

	protected:

		LPSTR m_szName;
		int   m_nElementCount;
	
};

#endif // CHORDGROUP_H

