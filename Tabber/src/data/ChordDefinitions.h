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
		void save              ();

		void        addChordGroup      (ChordGroup* ) ;
		
		inline int         getChordGroupCount ()          const { return getElementCount();   }
		inline ChordGroup* getChordGroupAt    (int index)       { return getElementAt(index); }

	protected:

		void loadDefaultValues ();
		
	private:

		static const char CHORD_DEFINITIONS_FILE_NAME[];
		             char _chordDefinitionsFileFullName[MAX_PATH];

		static const int  MAX_GROUP_COUNT;

};

#endif // CHORDDEFINITIONS_H

