#ifndef MEMORYWATCHER_H
#define MEMORYWATCHER_H

#include <windows.h>

#include "../util/DebugWindow.h"


typedef struct
{
	DWORD	address;
	DWORD	size;
	char	file[64];
	DWORD	line;
} ALLOC_INFO;


/**
 * A tool to trace memory allocations and detect leaks
 * @see http://www.flipcode.com/tutorials/tut_memleak.shtml
 */
class MemoryWatcher
{

	public:

		MemoryWatcher();
		~MemoryWatcher();
		
		void addTrack(DWORD, DWORD, const char*, DWORD );
		void removeTrack(DWORD );
		void report() const;
	    
	private:
	
		bool isTrackingSafe; //< Memory tracking might not safe at certain points of the program. In those cases, tracking can be disabled by hand.
};


#endif // MEMORYWATCHER_H

