#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../../resource.h"
#include "../core/utilities.h"
#include "../core/Application.h"
#include "../core/GlobalSettings.h"


class CMainWindow
{

	public:

		CMainWindow(CApplication *);
		virtual ~CMainWindow();

		void create(HINSTANCE , int , int , int , int );
		void show  (int );

	protected:

		static LRESULT CALLBACK WindowProc(HWND , UINT , WPARAM , LPARAM );
	
		void onCreate  ( );
		void onClose   ( );
		void onCommand (WPARAM , LPARAM );

	protected:

		static const char SZ_CLASS_NAME[];

		CApplication* m_lpApplication;
		HWND          m_hWindow;

};


// required because WindowProc must be static
extern CMainWindow* g_lpMainWindow; 


#endif // MAIN_WINDOW_H
