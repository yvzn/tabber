#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "../util/utilities.h"


class StatusBar
{

	public:

		StatusBar();
		~StatusBar();

		void create       (HWND );
		void resize ( );
		RECT getSize();
		
		void setTextInPart(int , const char * );

	protected:
	
		HWND _hWindow;
};

#endif // STATUSBAR_H

