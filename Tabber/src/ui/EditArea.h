#ifndef EDITAREA_H
#define EDITAREA_H

#include "../util/utilities.h"

/* forward */ class MainWindow;


class EditArea
{

	public:

		EditArea(MainWindow* );
		~EditArea();

		void create (HWND );
		void resize (const RECT& );

		void loadContentFrom (const char* );
		void saveContentTo   (const char* );
		void wipeContent     ( );

		void doCommand(UINT );

	protected:

		static LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		       LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

	protected:

		MainWindow* _mainWindow;
		
		WNDPROC  _superClassWindowProc;
		HWND     _hWindow;
		
};

#endif // EDITAREA_H

