#ifndef CHORDSTABCONTROL_H
#define CHORDSTABCONTROL_H

#include "../util/utilities.h"
#include "../core/Application.h"
#include "../ui/ChordsToolbar.h"

/* forward */ class MainWindow;


class ChordsTabControl
{

	public:

		ChordsTabControl(MainWindow* );
		~ChordsTabControl();

		void create(HWND );
		void resize(const RECT& );
		
		void updateOnTabChange();

		inline MainWindow* getMainWindow() const { return _mainWindow; }

	public:
	
		static const int CHORDS_TOOLBAR_HEIGHT;

	protected:

		void createTabs(HWND );
		void createChildPanels(HWND );
		
	protected:

		MainWindow* _mainWindow;
	
		HWND  _hWindow;

		ChordsTabControlPanel** _panels;
		int                     _panelCount;
		int                     _activePanelIndex;

};

#endif // CHORDSTABCONTROL_H

