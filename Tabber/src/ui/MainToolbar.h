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

		void setCommandEnabled(int , bool );
		
	protected:
	
		HWND _hWindow;
		int  _buttonCount;
		
};


#endif // MAINTOOLBAR_H

