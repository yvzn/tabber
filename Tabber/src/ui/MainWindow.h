#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../util/utilities.h"

#include "../core/Application.h"
#include "../core/ApplicationSettings.h"

#include "../ui/MainToolbar.h"
#include "../ui/StatusBar.h"
#include "../ui/ChordsToolbar.h"
#include "../ui/EditArea.h"
#include "../ui/DocumentInterface.h"
#include "../ui/SettingsInterface.h"
#include "../ui/AboutDialog.h"


class MainWindow
{

	public:

		MainWindow(Application *);
		virtual ~MainWindow();

		void create(HINSTANCE ) ;
		void show  (int ) ;

		void setWindowTitle    (const char* );
		void setCommandEnabled (int , bool );

		inline Application*       getApplication       () { return _application;       }
		inline DocumentInterface* getDocumentInterface () { return _documentInterface; }

		inline EditArea*          getEditArea          () { return _editArea;          }
		inline StatusBar*         getStatusBar         () { return _status;            }
		inline HWND&              getWindowHandle      () { return _hWindow;           }
  		inline HMENU              getMenu              () { return GetMenu(_hWindow);  }

	public:
	
		static const char APPLICATION_NAME[];
		// No description
		
	protected:

		static LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		       LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		void onCommand  (WPARAM , LPARAM );
		void onNotify   (WPARAM , LPARAM );

		void onCreate   (HWND );
		void onClose    ( );
		void onDestroy  ( );
		void onSize     ( );
		void onDropFiles(HDROP );

	protected:

		static const char WINDOW_CLASS_NAME[];

		Application*       _application;
		DocumentInterface* _documentInterface;
		SettingsInterface* _settingsInterface;

		MainToolbar*       _toolbar;
		StatusBar*         _status;
		ChordsToolbar*     _chordsToolbar;
		EditArea*          _editArea;

		HWND          _hWindow;

};


#endif // MAIN_WINDOW_H
