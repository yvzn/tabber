#include "Notify.h"


void CNotify::error(LPCSTR szErrorMsg)
{
	MessageBox (
		HWND_DESKTOP,
		szErrorMsg,
		"An error occured !",
		MB_ICONERROR | MB_OK );
}


void CNotify::debug(LPCSTR szFormat, ...)
{
    va_list ap;
    char szBuffer[1000];
    
    va_start(ap, szFormat);
    wvsprintf(szBuffer, szFormat, ap );
    va_end(ap);
    
	MessageBox (
		HWND_DESKTOP,
		szBuffer,
		"Notification",
		MB_ICONEXCLAMATION | MB_OK );    
}

