#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <windows.h>
#include "../../resource.h"
#include "../util/NotifyMessage.h"


class DebugWindow
{

	public:

		static void trace(const char* , ...);
		static void traceWindowsLastError();
		static void alert(const char*, ...);

		static void copyTrace();

	protected:

		static const char WINDOW_CLASS_NAME[];

		static void create(HINSTANCE );
	    static LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );
		static void writeln(const char* );
	    
	    static HWND _hWindow;
	    static bool _isWindowClassNotRegistered;

};


#endif // DEBUGWINDOW_H

