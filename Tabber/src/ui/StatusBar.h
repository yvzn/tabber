#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "../util/utilities.h"
#include "../ui/tools/EditionModes.h"


class StatusBar
{

	public:

		StatusBar();
		~StatusBar();

		void create  (HWND );
		void resize  ( );
		RECT getSize ( );

		void updateTypingMode(TypingMode );

	protected:

		void setTextInPart(int , const char * );

	protected:
	
		HWND  _hWindow;
		char* _typingModeString;
};

#endif // STATUSBAR_H

