#ifndef CHORDBUTTONSPANEL_H
#define CHORDBUTTONSPANEL_H

#include "../util/utilities.h"

/* forward */ class ChordsToolbar;


/**
 * A basic container for buttons, with the ability to handle WM_COMMANDs corresponding to chords.
 */
class ChordButtonsPanel
{

	public:

		ChordButtonsPanel(ChordsToolbar* );
		~ChordButtonsPanel();

		void create(HWND );
		void hide( );
		void show( );

		HWND& getWindowHandle();

		void addButton(const char* , int );

	protected:

		static LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		       LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

	protected:
	
		static const int CHORD_BUTTON_HEIGHT;
		static const int CHORD_BUTTON_WIDTH;
		static const int CHORD_BUTTON_PADDING;

		ChordsToolbar* _toolbar;
		
		WNDPROC  _superClassWindowProc;
		HWND     _hWindow;
	
		int _xNextButtonPosition;
};


#endif // CHORDBUTTONSPANEL_H

