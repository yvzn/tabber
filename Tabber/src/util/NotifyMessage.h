#ifndef NOTIFYMESSAGE_H
#define NOTIFYMESSAGE_H

#include <windows.h>


class NotifyMessage
{

	public:

		static void publicError     (HWND hParentWindow, const char* format, ...);
		static void alert           (HWND hParentWindow, const char* format, ...);
		static int  assertionFailed (const char* , const char* , int );

};


#endif // NOTIFYMESSAGE_H
