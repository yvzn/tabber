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

		static LRESULT CALLBACK WindowProc(HWND , UINT , WPARAM , LPARAM );

	protected:
	
		static const char WINDOW_CLASS_NAME[];
		static const int CHORD_BUTTON_HEIGHT;
		static const int CHORD_BUTTON_WIDTH;
		static const int CHORD_BUTTON_PADDING;
		
		WNDCLASSEX _windowClass;
		HWND       _hWindow;
	
		int _xNextButtonPosition;
};


// required because WindowProc must be static
extern MainWindow* gMainWindow; 


#endif // CHORDBUTTONSPANEL_H

