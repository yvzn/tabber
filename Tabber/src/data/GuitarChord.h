#ifndef GUITARCHORD_H
#define GUITARCHORD_H

#include "../util/utilities.h"

#define CHORD_DEPTH 6


class GuitarChord
{

	public:

		GuitarChord(LPCSTR , LPCSTR );
		virtual ~GuitarChord();
		
		const char* getName  ()      const; 
		const char* getNotes ()      const;
		      char  getNote  (int )  const; 
		
	protected:

		char* _name;
		char* _notes;

};

#endif // GUITARCHORD_H

