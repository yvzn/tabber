#ifndef UTILITIES_H
#define UTILITIES_H

#include <windows.h>
#include <commctrl.h> //toolbars, statusbars, tab controls..

#include "../../resource.h"

#include "../util/NotifyMessage.h"
#include "../util/RuntimeException.h"
#include "../util/DebugWindow.h"


// DEBUG ----------------------------------------------------------------------

// minigw's debug tools seems to screw up in Win32 (becauses they use the console ?)
// so I override them and define my own stuff
// -- these features should be disabled under MSVC++


#define __MY_DEBUG


#ifdef __MY_DEBUG


// memory watching: I override C++'s new and delete operators to log memory allocations
#include "../util/MemoryWatcher.h"
extern MemoryWatcher memoryWatcher;

#define ALLOCATE \
	void *ptr = (void *)malloc(size); \
	memoryWatcher.addTrack((DWORD)ptr, size, file, line); \
	return(ptr);
	
inline void * __cdecl operator new(unsigned int size, const char *file, int line) { ALLOCATE; }
inline void * __cdecl operator new[] (unsigned int size, const char *file, int line) { ALLOCATE; }

#define FREE \
	memoryWatcher.removeTrack((DWORD)p); \
	free(p);

inline void __cdecl operator delete(void *p) { FREE; }
inline void __cdecl operator delete[] (void *p) { FREE; }
 
#define DEBUG_NEW new(__FILE__, __LINE__)


// assertions
void __assertion_failed (const char*, const char*, int);
#define assert(condition) (condition) ? (void)0 : __assertion_failed(#condition, __FILE__, __LINE__)


// debug operations, before and after execution
#define DEBUG_START \
	MemoryWatcher memoryWatcher;

#define DEBUG_STOP \
	memoryWatcher.report();


#else //__MY_DEBUG


#define DEBUG_NEW new

#define assert(condition)

#define DEBUG_START
#define DEBUG_STOP


#endif //__MY_DEBUG


#define OBJECT_CREATED // good ol' personnal macros, not used here
#define OBJECT_DELETED 

#define new DEBUG_NEW


// OTHER UTILITIES ------------------------------------------------------------

void GetAbsoluteFileName(char* , const char* );
void ApplyUsersDefaultFont(HWND );


#endif // UTILITIES_H
