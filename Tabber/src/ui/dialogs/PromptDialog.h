#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

#include "../../util/utilities.h"


class PromptDialog
{
    
	public:

	    static int prompt(HWND , const char* , int );
	    static int getIntegerValue();
	    
	    static BOOL CALLBACK handleMessage(HWND, UINT, WPARAM, LPARAM);
	    
	protected:

		static const char* _promptString;
		static int         _returnValue;

};

#endif // PROMPTDIALOG_H

