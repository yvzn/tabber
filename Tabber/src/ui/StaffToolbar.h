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

		LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

};

#endif // STAFFTOOLBAR_H

