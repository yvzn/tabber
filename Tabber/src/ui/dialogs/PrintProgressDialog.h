#ifndef PRINTPROGRESSDIALOG_H
#define PRINTPROGRESSDIALOG_H

#include "../../util/utilities.h"


/**
 * Shows a dialog where current printing job progress is shown with the possibility to cancel printing
 */
class PrintProgressDialog
{
    
	public:

	    static void show(HWND , const char* , int );
		static void hide();

	    static bool didUserCancelPrinting();
		static void setCurrentPage(int page);
	    
	    static BOOL CALLBACK handleMessage(HWND, UINT, WPARAM, LPARAM);
	    
	protected:

		static HWND        _hWindow;

		static const char* _documentName;
		static char        _status[128];
		static int         _totalPages;
		static bool        _continuePrinting;

};

#endif // PRINTPROGRESSDIALOG_H

