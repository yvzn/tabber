#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include "../util/utilities.h"


class MainToolbar
{

	public:

		MainToolbar();
		~MainToolbar();

		void create  (HWND );
		void resize  ( );
		RECT getSize ( );
		
	protected:
	
		HWND _hWindow;
		
};


#endif // MAINTOOLBAR_H

