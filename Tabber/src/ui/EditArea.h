#ifndef EDITAREA_H
#define EDITAREA_H

#include "../util/utilities.h"
#include "../core/ApplicationSettings.h"
#include "../ui/EditionToolkit.h"

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

		inline MainWindow* getMainWindow() const { return _mainWindow; }

	protected:

		static LRESULT CALLBACK forwardMessage(HWND , UINT , WPARAM , LPARAM );
		       LRESULT CALLBACK handleMessage(HWND , UINT , WPARAM , LPARAM );

		void    onSelectionChange   ( );
		void    onDocumentModified  ( );
		void    onKeyUp             (int );
		LRESULT onCharSpecialMode   (int );
		LRESULT onCharOverwriteMode (int);

	protected:

		MainWindow* _mainWindow;
		
		WNDPROC  _superClassWindowProc;
		HWND     _hWindow;
		HFONT    _displayFont;
		
		EditionToolkit* _toolkit;

};


#endif // EDITAREA_H

