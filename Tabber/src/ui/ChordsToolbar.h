#ifndef CHORDSTOOLBAR_H
#define CHORDSTOOLBAR_H

#include "../util/utilities.h"
#include "../core/Application.h"
#include "../ui/ChordButtonsPanel.h"


/* forward */ class MainWindow;


class ChordsToolbar
{

	public:

		ChordsToolbar(MainWindow* );
		~ChordsToolbar();

		void create(HWND );
		void resize(const RECT& );
		
		void updateOnTabChange();

		inline MainWindow* getMainWindow() const { return _mainWindow; }

	public:
	
		static const int CHORDS_TOOLBAR_HEIGHT;
		
	protected:
	
		MainWindow* _mainWindow;
	
		HWND  _hWindow;

		ChordButtonsPanel** _panels;
		int                 _panelCount;
		int                 _activePanelIndex;

};

#endif // CHORDSTOOLBAR_H

