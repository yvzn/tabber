#ifndef STAFFTOOLBAR_H
#define STAFFTOOLBAR_H

#include "../ui/ChordsTabControlPanel.h"

/*
 * No description
 */
class StaffToolbar : public ChordsTabControlPanel
{

	public:

		StaffToolbar(ChordsTabControl* );
		~StaffToolbar();

		void create(HWND );

		void setCommandEnabled(int , bool );

	protected:

		LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		inline int getButtonHeight  ( ) { return 25; }
		inline int getButtonWidth   ( ) { return 75; }
		virtual int getButtonHorizontalPadding ( ) { return 2; }
		virtual int getButtonVerticalPadding   ( ) { return 2; }

};

#endif // STAFFTOOLBAR_H

