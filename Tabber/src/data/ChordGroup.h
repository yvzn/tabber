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

		const char* getName() const;
		
		void          addChord     (GuitarChord* );
		GuitarChord*  getChordAt   (int );
		int           getChordCount() const; 

	protected:

		char*  _name;
	
};

#endif // CHORDGROUP_H

