#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../util/utilities.h"

#include "../core/Application.h"
#include "../core/ApplicationSettings.h"

#include "../ui/MainToolbar.h"
#include "../ui/StatusBar.h"
#include "../ui/ChordsTabControl.h"
#include "../ui/EditArea.h"
#include "../ui/StaffToolbar.h"

#include "../ui/tools/DocumentInterface.h"
#include "../ui/tools/SettingsInterface.h"
#include "../ui/tools/PrinterInterface.h"

#include "../ui/dialogs/AboutDialog.h"




class MainWindow
{

	public:

		MainWindow(Application *);
		virtual ~MainWindow();

		void create(HINSTANCE ) ;
		void show  (int ) ;

		void setWindowTitle    (const char* );
		void setCommandEnabled (int , bool );

		inline bool interceptMessage(MSG* message) { return _documentInterface->interceptMessage(message); }

		inline Application*       getApplication       () { return _application;       }
		inline DocumentInterface* getDocumentInterface () { return _documentInterface; }

		inline EditArea*          getEditArea          () { return _editArea;          }
		inline StatusBar*         getStatusBar         () { return _status;            }
		inline StaffToolbar*      getStaffToolbar      () { return _staff;             }

		inline HWND               getWindowHandle      () { return _hWindow;           }
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
		PrinterInterface*  _printerInterface;

		MainToolbar*       _toolbar;
		StatusBar*         _status;
		ChordsTabControl*  _chordsTabControl;
		EditArea*          _editArea;
		StaffToolbar*      _staff;

		HWND               _hWindow;

		unsigned int       _findReplaceCommandId;

};


#endif // MAIN_WINDOW_H
