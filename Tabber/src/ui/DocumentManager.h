#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include "../util/utilities.h"

/* forward */ class MainWindow;


/**
 * Storage class for document state
 */
class DocumentManager
{

	public:

		DocumentManager(MainWindow*);
		~DocumentManager();

		void setDocumentModified(bool);
		bool continueIfDocumentModified();		
		
		void onNewDocument();
		bool onDocumentSave();
		bool onDocumentSaveAs();
		void onDocumentOpen();

	protected:

		void blankDocumentFlags();
		void updateMainWindowTitle();
		void updateFileName();
	
	protected:

		MainWindow* _mainWindow;

		bool _isDocumentModified;
		bool _isFileLoaded;
		char _filePathAndName[MAX_PATH];
		char* _fileName;
		
		OPENFILENAME _fileDialogOptions;
		
};

#endif // DOCUMENTMANAGER_H

