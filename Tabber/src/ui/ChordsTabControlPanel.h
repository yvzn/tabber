#ifndef CHORDSTABCONTROLPANEL_H
#define CHORDSTABCONTROLPANEL_H

#include "../util/utilities.h"

/* forward */ class ChordsTabControl;


/**
 * Parent class for panels displayed in the chords tab control.
 */
class ChordsTabControlPanel
{

	public:

		ChordsTabControlPanel(ChordsTabControl* );
		virtual ~ChordsTabControlPanel();

		virtual void create(HWND );
		void hide( );
		void show( );

		HWND addButton(const char* , int , DWORD =0);

		inline HWND& getWindowHandle() { return _hWindow; }

	protected:

		static  LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		virtual LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM ) =0;

		virtual int getButtonHeight  ( ) =0;
		virtual int getButtonWidth   ( ) =0;
		virtual int getButtonHorizontalPadding ( ) =0;
		virtual int getButtonVerticalPadding   ( ) =0;

	protected:

		ChordsTabControl* _parent;

		WNDPROC  _superClassWindowProc;
		HWND     _hWindow;

		int _xNextButtonPosition;
};

#endif // CHORDSTABCONTROLPANEL_H

