#include "utilities.h"


/**
 * Build absolute file name using current directory
 * @param szAbsoluteFileName a string buffer where absolute filename will be stored
 * @param szRelativeFileName a null-terminated string representing a file name relative to current directory
 */
void GetAbsoluteFileName(LPSTR szAbsoluteFileName, LPCSTR szRelativeFileName)
{
	char szCurrentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurrentDirectory);
	lstrcpy(szAbsoluteFileName, szCurrentDirectory);
	lstrcat(szAbsoluteFileName, "\\");
	lstrcat(szAbsoluteFileName, szRelativeFileName);
}

