#ifndef CHORDDEFINITIONS_H
#define CHORDDEFINITIONS_H

#include "../util/utilities.h"
#include "../util/DynamicPointerArray.h"
#include "../util/IniFileSequentialParser.h"

#include "../data/GuitarChord.h"
#include "../data/ChordGroup.h"


class ChordDefinitions
{

	public:

		ChordDefinitions();
		~ChordDefinitions();
		
		void load              ()       ;
		void loadDefaultValues ()       ;
		void save              () const ;

		DynamicPointerArray<GuitarChord>* getChords();
		DynamicPointerArray<ChordGroup>*  getChordGroups();

	protected:

		static const char CHORD_DEFINITIONS_FILE_NAME[];
		
		char                              _chordDefinitionsFileFullName[MAX_PATH];
		DynamicPointerArray<GuitarChord>* _chords;
		DynamicPointerArray<ChordGroup>*  _chordGroups;

};

#endif // CHORDDEFINITIONS_H

