#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../util/utilities.h"

#include "../core/Application.h"
#include "../core/ApplicationSettings.h"

#include "../ui/components.h"


class MainWindow
{

	public:

		MainWindow(Application *);
		virtual ~MainWindow();

		void create(HINSTANCE ) ;
		void show  (int )       const ;

	protected:

		static LRESULT CALLBACK WindowProc(HWND , UINT , WPARAM , LPARAM );
	
		void onCreate  (HWND );
		void onClose   ( );
		void onSize    ( );
		void onCommand (WPARAM , LPARAM );

	protected:

		static const char WINDOW_CLASS_NAME[];

		Application*  _application;
		MainToolbar*  _toolbar;
		StatusBar*    _status;
		
		HWND          _hWindow;

};


// required because WindowProc must be static
extern MainWindow* gMainWindow; 


#endif // MAIN_WINDOW_H
