#ifndef EDITAREA_H
#define EDITAREA_H

#include "../util/utilities.h"

/* forward */ class MainWindow;


class EditArea
{

	public:

		EditArea(MainWindow* );
		~EditArea();

		void create(HWND );
		void resize(const RECT&);
		
	protected:

		MainWindow* _mainWindow;
		
		HWND _hWindow;
		
};

#endif // EDITAREA_H

