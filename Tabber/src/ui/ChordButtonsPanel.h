#ifndef CHORDBUTTONSPANEL_H
#define CHORDBUTTONSPANEL_H

#include "../util/utilities.h"

/* forward */ class MainWindow;


/**
 * A basic container for buttons, with the ability to handle/propagate
 * WM_COMMANDs corresponding to chords.
 */
class ChordButtonsPanel
{

	public:

		ChordButtonsPanel();
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
		
		WNDPROC  _superClassWindowProc;
		HWND     _hWindow;
	
		int _xNextButtonPosition;
};


// required because WindowProc must be static
extern MainWindow* gMainWindow; 


#endif // CHORDBUTTONSPANEL_H

