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

		HWND addButton(const char* , int , DWORD =0);

	protected:

		LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		inline int getButtonHeight  ( ) { return 25; }
		inline int getButtonWidth   ( ) { return 50; }
		virtual int getButtonHorizontalPadding ( ) { return 2; }
		virtual int getButtonVerticalPadding   ( ) { return 2; }

};


#endif // CHORDSTOOLBAR_H

