#ifndef TUNINGDEFINITIONS_H
#define TUNINGDEFINITIONS_H

#include "../util/utilities.h"
#include "../data/GuitarChord.h"
#include "../util/DynamicPointerArray.h"
#include "../util/IniFileSequentialParser.h"

// Tunings are stored as guitar chords
typedef GuitarChord GuitarTuning;


class TuningDefinitions: protected DynamicPointerArray<GuitarTuning>
{

	public:

		TuningDefinitions();
		~TuningDefinitions();

		void load ();
		void save ();

		inline GuitarTuning*  getTuningAt   (int index) { return getElementAt(index); }
		inline int            getTuningCount() const    { return getElementCount();   }

	private:

		static const char TUNING_DEFINITIONS_FILE_NAME[];
		             char _tuningDefinitionsFileFullName[MAX_PATH];

};


#endif // TUNINGDEFINITIONS_H

