#ifndef CHORDSTOOLBAR_H
#define CHORDSTOOLBAR_H

#include "../util/utilities.h"
#include "../core/Application.h"


/* forward */ class MainWindow;


class ChordsToolbar
{

	public:

		ChordsToolbar(MainWindow* );
		~ChordsToolbar();

		void create(HWND );
		void resize(const RECT& );
		
		void updateOnTabChange();

		static const int CHORDS_TOOLBAR_HEIGHT;
	
	protected:
	
		void hidePanel(int );
		void showPanel(int );

	protected:
	
		MainWindow* _mainWindow;
	
		HWND  _hWindow;

		int   _panelCount;
		HWND* _panels;
		int   _activePanelIndex;

		
};

#endif // CHORDSTOOLBAR_H

