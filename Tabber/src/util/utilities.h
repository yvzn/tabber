#ifndef UTILITIES_H
#define UTILITIES_H

#include <windows.h>
#include <commctrl.h> //toolbars, statusbars, tab controls..

#include "../../resource.h"

#include "../util/NotifyMessage.h"
#include "../util/RuntimeException.h"


// DEBUG ----------------------------------------------------------------------

#define __MY_DEBUG


#ifdef __MY_DEBUG

// object creation/deletion count (avoid leaks :)
extern UINT gObjectCount;
#define OBJECT_CREATED ++gObjectCount
#define OBJECT_DELETED --gObjectCount

// minigw's assert screws up in Win32 becauses it uses the console, so I have defined my own
void __assertion_failed (const char*, const char*, int);
#define assert(condition) (condition) ? (void)0 : __assertion_failed(#condition, __FILE__, __LINE__)

//misc. debug initialisations and messages
#define DEBUG_START \
	UINT gObjectCount = 0;

#define DEBUG_STOP \
	NotifyMessage::debug("delta(created objects, deleted objects):\n%d object(s) not released", gObjectCount);


#else //__MY_DEBUG

#define OBJECT_CREATED 
#define OBJECT_DELETED 

#define assert(condition)

#define DEBUG_START
#define DEBUG_STOP


#endif //__MY_DEBUG


// OTHER UTILITIES ------------------------------------------------------------

void GetAbsoluteFileName(char* , const char* );


#endif // UTILITIES_H
