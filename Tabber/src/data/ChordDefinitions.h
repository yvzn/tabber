#ifndef CHORDDEFINITIONS_H
#define CHORDDEFINITIONS_H

#include "../util/utilities.h"
#include "../util/DynamicPointerArray.h"
#include "../util/IniFileSequentialParser.h"
#include "../data/ChordGroup.h"


class ChordDefinitions: protected DynamicPointerArray<ChordGroup>
{

	public:

		ChordDefinitions();
		~ChordDefinitions();
		
		void load              ();
		void loadDefaultValues ();
		void save              ();

		void        addChordGroup      (ChordGroup* ) ;
		int         getChordGroupCount ()             const;
		ChordGroup* getChordGroupAt    (int )         ;
		
	protected:

		static const char CHORD_DEFINITIONS_FILE_NAME[];
		
		char _chordDefinitionsFileFullName[MAX_PATH];

};

#endif // CHORDDEFINITIONS_H

