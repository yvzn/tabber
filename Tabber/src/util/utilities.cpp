#include "utilities.h"


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

