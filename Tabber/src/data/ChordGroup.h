#ifndef CHORDGROUP_H
#define CHORDGROUP_H

#include "../util/utilities.h"
#include "../util/DynamicPointerArray.h"
#include "../data/GuitarChord.h"


class ChordGroup: protected DynamicPointerArray<GuitarChord>
{

	public:

		ChordGroup(const char*);
		virtual ~ChordGroup();

		inline const char* getName() const { return _name; }
		
		void addChord     (GuitarChord* );
		
		inline GuitarChord*  getChordAt   (int index) { return getElementAt(index); }
		inline int           getChordCount() const    { return getElementCount();   }

	protected:

		static const int  MAX_CHORD_COUNT;

		char*  _name;
	
};

#endif // CHORDGROUP_H

