#ifndef DOCUMENTINTERFACE_H
#define DOCUMENTINTERFACE_H

#include "../../util/utilities.h"

/* forward */ class MainWindow;


/**
 * Storage class for document state
 */
class DocumentInterface
{

	public:

		DocumentInterface(MainWindow*);
		~DocumentInterface();

		void setDocumentModified(bool);
		bool continueIfDocumentModified();		
		
		void onNewDocument    ( );
		bool onDocumentSave   ( );
		bool onDocumentSaveAs ( );
		bool onDocumentOpen   ( );
		bool onDocumentOpen   (const char* );

		void onFind    ( );
		void onFindNext( );
		void onReplace ( );

		inline const char* getFileName() const { return _fileName; }

		inline bool interceptMessage(MSG* message) { return TRUE == IsDialogMessage(_hFindReplaceDialog, message); }

	protected:

		void blankDocumentFlags();
		void updateMainWindowTitle();
		void updateFileName();
		bool loadSpecifiedDocument();
	
		static UINT APIENTRY handleFindReplaceDialogMessage(HWND , UINT , WPARAM , LPARAM );

	protected:

		MainWindow* _mainWindow;

		OPENFILENAME _fileDialogOptions;

		bool  _isDocumentModified;
		bool  _isFileLoaded;
		char  _filePathAndName[MAX_PATH];
		char* _fileName;
		
		FINDREPLACE _findReplaceOptions;

		HWND  _hFindReplaceDialog;
		static bool _isDialogOpened;
		char* _findWhat;
		char* _replaceWith;
};

#endif // DOCUMENTINTERFACE_H

