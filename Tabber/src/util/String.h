#ifndef MSTRING_H
#define MSTRING_H

#include <windows.h>
#include <cstring>
#include <cctype>


/**
 * Various utilities related to strings
 */
class String
{
	public:

		static const char* toString(int );
		static       char* stristr(const char* , const char* );

	protected:

		static char _conversionBuffer[20];
};

#endif // MSTRING_H

