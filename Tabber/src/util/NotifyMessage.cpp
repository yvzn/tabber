#include "NotifyMessage.h"


#define DECLARE_MESSENGER_BODY(icon, hWindow)   \
{                                               \
    va_list ap;                                 \
    char* buffer = new char[1000];              \
                                                \
    va_start(ap, format);                       \
    wvsprintf(buffer, format, ap );             \
    va_end(ap);                                 \
                                                \
	MessageBox (                                \
		hWindow,                                \
		buffer,                                 \
		"Tabber",                               \
		icon | MB_OK );                         \
                                                \
	delete [] buffer;                           \
}


void NotifyMessage::publicError(HWND hParentWindow, const char* format, ...)
DECLARE_MESSENGER_BODY(MB_ICONERROR, hParentWindow);


void NotifyMessage::alert(HWND hParentWindow, const char* format, ...)
DECLARE_MESSENGER_BODY(MB_ICONINFORMATION, hParentWindow);


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

