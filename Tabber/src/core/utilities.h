#ifndef UTILITIES_H
#define UTILITIES_H

#include <windows.h>
#include <assert.h>
#include "Notify.h"
#include "RuntimeException.h"


#undef __DEBUG

#ifdef __DEBUG
extern UINT g_uObjectCount;
#define OBJECT_CREATED ++g_uObjectCount;
#define OBJECT_DELETED --g_uObjectCount;
#else
#define OBJECT_CREATED ;
#define OBJECT_DELETED ;
#endif //__DEBUG


void GetAbsoluteFileName(LPSTR , LPCSTR );


#endif // UTILITIES_H
