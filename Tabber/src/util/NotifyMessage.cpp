#include "NotifyMessage.h"
#include "../ui/MainWindow.h"


/**
 * Public errors (ie. visible to end-user)
 */
void NotifyMessage::publicError(const char* errorMsg)
{
	MessageBox (
		HWND_DESKTOP,
		errorMsg,
		"An error occured !",
		MB_ICONERROR | MB_OK );
}


void NotifyMessage::debug(const char* format, ...)
{
    va_list ap;
    char* buffer = new char[1000];
    
    va_start(ap, format);
    wvsprintf(buffer, format, ap );
    va_end(ap);
    
	MessageBox (
		HWND_DESKTOP,
		buffer,
		"Debug : Notification",
		MB_ICONEXCLAMATION | MB_OK );    
		
	delete [] buffer;
}


int NotifyMessage::assertionFailed(const char* expr, const char* file, int line)
{
    char* buffer = new char[1000];
	wsprintf(buffer, "The followong assertion failed.\n\n%s\n\nFile: %s\nLine: %d", expr, file, line);

	int result = MessageBox (
		HWND_DESKTOP,
		buffer,
		"Debug : Assertion Failed !",
		MB_ICONERROR | MB_ABORTRETRYIGNORE );
		
	delete [] buffer;
	return result;
}

