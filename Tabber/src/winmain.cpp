#include <windows.h>

#include "core/Application.h"
#include "util/utilities.h"


DEBUG_START;

int WINAPI WinMain (
	HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow )
{
	MSG msg;
 	Application* application = new Application();
	
	try
	{
		application->create(hThisInstance);
		application->show(nCmdShow);
		application->processCommandLine(lpCmdLine);
	
		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
   			if(!application->translateAccelerator(&msg)) // handle keyboard shortcuts
			{
   				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	catch(RuntimeException* exception)
	{
		RuntimeException ex("::Winmain", exception);
		NotifyMessage::publicError(ex.getStackTrace());
	}
	
	delete application;

DEBUG_STOP;

	return msg.wParam;
}
