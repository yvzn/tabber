#include "NotifyMessage.h"


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

