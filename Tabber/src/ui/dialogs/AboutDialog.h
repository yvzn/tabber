#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "../../util/utilities.h"


class AboutDialog
{
    
	public:

	    static int show(HWND );
	    
	    static BOOL CALLBACK handleMessage(HWND, UINT, WPARAM, LPARAM);

};

#endif // ABOUTDIALOG_H

