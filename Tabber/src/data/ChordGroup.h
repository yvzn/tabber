#ifndef CHORDGROUP_H
#define CHORDGROUP_H

#include "../util/utilities.h"


class ChordGroup
{

	public:

		ChordGroup(const char* , int);
		virtual ~ChordGroup();

		const char* getName() const;
		int getElementCount() const; 
		void setElementCount(int ); 

	protected:

		char*  _name;
		int    _elementCount;
	
};

#endif // CHORDGROUP_H

