#ifndef NOTIFYMESSAGE_H
#define NOTIFYMESSAGE_H

#include <windows.h>


class NotifyMessage
{

	public:

		static void publicError(const char* );
		
		static void debug(const char* , ...);
		static int assertionFailed(const char* , const char* , int );

};


#endif // NOTIFYMESSAGE_H
