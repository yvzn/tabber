#include "System.h" 

char System::_pathToExe[MAX_PATH] = "";
char System::_localeStringBuffer[LOCALE_STRING_MAX_LENGTH] = "";


/**
 * Builds an absolute path to specified file name using Exe directory
 * @param absoluteFileName a string buffer where absolute filename will be stored
 * @param relativeFileName a null-terminated string representing a file name relative to current directory
 */
void System::getAbsoluteFileName(char* absoluteFileName, const char* relativeFileName)
{
	if(_pathToExe[0] == '\0') //retrieve at first call
	{
		DWORD copiedCharsCount = GetModuleFileName(NULL, _pathToExe, MAX_PATH);
		if(copiedCharsCount == 0)
		{
			throw new RuntimeException("::GetAbsoluteFileName", "Could not find executable location");
		}

 		//find the last "\\" (limit between path and .exe name)
 		int limit;
  		for(limit = copiedCharsCount-1; _pathToExe[limit] != '\\' && limit > -1; limit--)
  			;

		//cut string down to path
		_pathToExe[limit] = '\0';
	}

	lstrcpy(absoluteFileName, _pathToExe);
	lstrcat(absoluteFileName, "\\");
	lstrcat(absoluteFileName, relativeFileName);
}


/**
 * Applies the default GUI font (as specified by the user in Windows's display settings) to a control
 */
void System::applyGUIDefaultFont(HWND hControl)
{
	static HFONT defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	SendMessage(hControl, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE, 0));
}


/**
 * Loads the localized string for the specified resource identifier / command Id
 * @warning returned pointer is volatile, client should copy data if it needs a longer term use.
 */
const char* System::getLocaleString(unsigned int resourceId)
{
	LoadString(
		GetModuleHandle(NULL),
		resourceId,
		_localeStringBuffer,
		LOCALE_STRING_MAX_LENGTH );

	return _localeStringBuffer;
}

