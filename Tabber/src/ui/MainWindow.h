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

		Application*     getApplication();
		EditArea*        getEditArea();
		DocumentInterface* getDocumentInterface();

		HWND& getWindowHandle();
		
		void setWindowTitle(const char* );

	public:
	
		static const char APPLICATION_NAME[];
		
	protected:

		static LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		       LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		void onCreate   (HWND );
		void onClose    ( );
		void onSize     ( );
		void onCommand  (WPARAM , LPARAM );
		void onNotify   (WPARAM , LPARAM );

	protected:

		static const char WINDOW_CLASS_NAME[];

		Application*       _application;
		MainToolbar*       _toolbar;
		StatusBar*         _status;
		ChordsToolbar*     _chordsToolbar;
		EditArea*          _editArea;
		DocumentInterface* _documentInterface;
		SettingsInterface* _settingsInterface;
		
		HWND          _hWindow;

};


#endif // MAIN_WINDOW_H
