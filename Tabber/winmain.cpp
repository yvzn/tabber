#include <windows.h>

#include "src/core/utilities.h"
#include "src/core/Application.h"

//*
#ifdef __DEBUG
UINT g_uObjectCount = 0;
#endif //__DEBUG
//*/

int WINAPI WinMain (
	HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow )
{
	MSG msg;
 	CApplication* lpApplication = new CApplication();
	
 	try
	{
		lpApplication->create(hThisInstance);
		lpApplication->show(nCmdShow);
	
		while(GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	catch(CRuntimeException* lpException)
	{
		CRuntimeException ex("::Winmain", lpException);
		CNotify::error(ex.getStackTrace());
	}
	
	delete lpApplication;

//*
#ifdef __DEBUG
	CNotify::debug("object count: %d", g_uObjectCount);
#endif //__DEBUG
//*/
	
	return msg.wParam;
}
