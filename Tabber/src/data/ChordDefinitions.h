#ifndef CHORDDEFINITIONS_H
#define CHORDDEFINITIONS_H

#include <iostream>
#include <fstream>

#include "../core/utilities.h"
#include "../core/DynamicArray.h"
#include "../data/Chord.h"
#include "../data/ChordGroup.h"


class CChordDefinitions
{

	public:

		CChordDefinitions();
		~CChordDefinitions();
		
		void load              ();
		void loadDefaultValues ();
		void save              ();

	protected:

		static const char SZ_CHORD_DEFINITIONS_FILE_NAME[];
		
		char                         m_szAbsoluteFileName[MAX_PATH];
		CDynamicArray<CChord>*       m_lpChords;
		CDynamicArray<CChordGroup>*  m_lpChordGroups;

};

#endif // CHORDDEFINITIONS_H

