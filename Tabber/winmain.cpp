#include <windows.h>

#include "src/core/Application.h"
#include "src/util/utilities.h"


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
	
		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	catch(RuntimeException* exception)
	{
		RuntimeException ex("::Winmain", exception);
		NotifyMessage::error(ex.getStackTrace());
	}
	
	delete application;

DEBUG_STOP;
	
	return msg.wParam;
}
