#ifndef SYSTEM_H
#define SYSTEM_H

#include <windows.h>

#include "../util/RuntimeException.h"

#define LOCALE_STRING_MAX_LENGTH 256



/**
 * Various utilities related to system and Windows's environment
 */
class System
{

	public:

		static void    getAbsoluteFileName (char* , const char* );
		static void    applyGUIDefaultFont (HWND );

		static const char* getLocaleString(unsigned int );

	protected:

		static char _pathToExe[MAX_PATH];
		static char _localeStringBuffer[LOCALE_STRING_MAX_LENGTH];

};

#endif // SYSTEM_H

