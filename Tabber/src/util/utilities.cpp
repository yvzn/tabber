#include "utilities.h"
#include <cmath>


/**
 * @throws a RuntimeException to abort the program "smoothly" if user decides so
 */
void __assertion_failed(const char* condition, const char* file, int line)
{
	if(NotifyMessage::assertionFailed(condition, file, line) != IDIGNORE )
	{
		throw new RuntimeException("::assert", "Assertion failed, program aborted by user");
	}
}


/**
 * Conversion from integer to string
 * @return a pointer to internal structure containing the result of the conversion. Client should not rely on permanent availability of this structure and should rather copy result in its own structure.
 */
const char* toString(int value)
{
	static char conversionBuffer[20];
	wsprintf(conversionBuffer, "%d", value);
	return (const char*)conversionBuffer;
}


/**
 * Case insensitive strstr
 */
char *stristr(const char* str1, const char* str2)
{
	char *s1 = const_cast<char*>(str1), *s2 = const_cast<char*>(str2);
	char *p, *ret;
 	int len1 = 0, len2 = 0, c1, c2, matches = 0;

	while(s1[len1]) len1++;
	while(s2[len2]) len2++;
 	if(len2 > len1) return 0;
	p = ret = s1;
	while(*p)
	{
		if((c1 = toupper((unsigned char)*p++))
			== (c2 = toupper((unsigned char)s2[matches++])))
		{
			if(matches == len2) return ret;
			continue;
		}
  		ret = p;
		if(matches = (c1 == toupper((unsigned char)*s2)) ? 1 : 0) ret--;
	}
	if(s2[matches]) return 0;
	return ret;
}




/**
 * Builds an absolute path to specified file name using Exe directory
 * @param absoluteFileName a string buffer where absolute filename will be stored
 * @param relativeFileName a null-terminated string representing a file name relative to current directory
 */
void GetAbsoluteFileName(char* absoluteFileName, const char* relativeFileName)
{
	static char pathToExe[MAX_PATH] = "\0";
	
	if(pathToExe[0] == '\0') //retrieve at first call
	{
		DWORD copiedCharsCount = GetModuleFileName(NULL, pathToExe, MAX_PATH);
		if(copiedCharsCount == 0)
		{
			throw new RuntimeException("::GetAbsoluteFileName", "Could not find executable location");
		}

 		//find the last "\\" (limit between path and .exe name)
 		int limit;
  		for(limit = copiedCharsCount-1; pathToExe[limit] != '\\' && limit > -1; limit--)
  			;

		//cut string down to path
		pathToExe[limit] = '\0';
	}
 	
	lstrcpy(absoluteFileName, pathToExe);
	lstrcat(absoluteFileName, "\\");
	lstrcat(absoluteFileName, relativeFileName);
}


/**
 * Applies the default GUI font (as specified by the user in Windows's display settings) to a control
 */
void ApplyUsersDefaultFont(HWND hControl)
{
	static HFONT defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	
	SendMessage(hControl, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE, 0));
}
