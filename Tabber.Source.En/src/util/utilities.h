/*
 * Tabber - Guitar Tablature Editor, freeware and open-source
 * Copyright (C) 2004  Yvan Razafindramanana
 *
 * Additionnal Contributors:
 * <place your name here :)>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef UTILITIES_H
#define UTILITIES_H


// VERSION --------------------------------------------------------------------

#define __PROGRAM_VERSION__ "2.0 (en) - DEBUG"



// INCLUDES -------------------------------------------------------------------

#include <windows.h>
#include <commctrl.h> //toolbars, statusbars, tab controls..
#include <cmath>

#include "../resource.h"

#include "../util/RuntimeException.h"
#include "../util/System.h"
#include "../util/String.h"
#include "../util/NotifyMessage.h"
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



#endif // UTILITIES_H
