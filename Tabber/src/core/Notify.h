#ifndef NOTIFY_H
#define NOTIFY_H

#include "../core/utilities.h"


class CNotify
{

	public:

		static void error(LPCSTR );
		static void debug(LPCSTR , ...);

};


#endif // NOTIFY_H
