#ifndef EDITAREA_H
#define EDITAREA_H

#include "../util/utilities.h"
#include "../core/ApplicationSettings.h"
#include "../ui/tools/EditionToolkit.h"
#include "../ui/tools/EditionActions.h"

/* forward */ class MainWindow;


class EditArea
{

	public:

		EditArea(MainWindow* );
		~EditArea();

		void create (HWND );
		void resize (const RECT& );

		void loadContentFrom (const char* );
		void saveContentTo   (const char* );
		void wipeContent     ( );

		void doCommand (UINT );

		void setFont       (const LOGFONT& );
		void setFocus      ( )              ;

		void onSelectAll    ( );
		void onDelete       ( );
		void onInsertStaff  ( );
		void onInsertTuning ( );
		void onInsertBar    ( );
		void onInsertChord  (unsigned int , ArpeggioDispatcher::Direction =ArpeggioDispatcher::NONE);

		void onSelectionChange ( );

		inline MainWindow*     getMainWindow () { return _mainWindow;  }
		inline EditionToolkit* getToolkit    () { return _toolkit;     }

		inline HWND   getWindowHandle () { return _hWindow;     }
		inline HFONT  getDisplayFont  () { return _displayFont; }

	protected:

		static LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		       LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		void    onDocumentModified  ( );
		LRESULT onKeyUp             (int );
		LRESULT onKeyDown           (int );
		LRESULT onCharSpecialMode   (int );
		LRESULT onCharOverwriteMode (int);

		void apply(StaffAction *);

	protected:

		MainWindow* _mainWindow;
		
		WNDPROC  _superClassWindowProc;
		HWND     _hWindow;
		HFONT    _displayFont;
		
		EditionToolkit* _toolkit;

};


#endif // EDITAREA_H

