#ifndef CHORDSTOOLBAR_H
#define CHORDSTOOLBAR_H

#include "../util/utilities.h"
#include "../ui/ChordsTabControlPanel.h"
#include "../ui/tools/EditionActions.h"


/**
 * A basic container for buttons, with the ability to process button clicks.
 */
class ChordsToolbar: public ChordsTabControlPanel
{

	public:

		ChordsToolbar(ChordsTabControl* );
		~ChordsToolbar();

		void create(HWND );

		void addButton(const char* , int );

	protected:

		LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

	protected:

		static const int CHORD_BUTTON_HEIGHT;
		static const int CHORD_BUTTON_WIDTH;
		static const int CHORD_BUTTON_PADDING;

		int _xNextButtonPosition;

};


#endif // CHORDSTOOLBAR_H

