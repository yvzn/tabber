#ifndef GUITARCHORD_H
#define GUITARCHORD_H

#include "../util/utilities.h"


class GuitarChord
{

	public:

		GuitarChord(const char* , const char* );
		virtual ~GuitarChord();
		
		inline const char* getName()      const { return _name;      }
		inline int         getNoteCount() const { return _noteCount; }
		
		const char* getNote  (int n)  const { assert(n<_noteCount); return _notes[n];}
		const char* getNotes ()       const;

		int getWidth() const { return _width; }

	protected:

		void createExtendedChord(const char* );

	protected:

		char*   _name;
		char**  _notes;
		int     _noteCount;
		bool    _isExtended;
		int     _width;      //< character count of the largest note in the tuning

	    static char conversionBuffer[40];

};

#endif // GUITARCHORD_H

