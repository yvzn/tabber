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

	protected:

		void blankDocumentFlags();
		void updateMainWindowTitle();
	
	protected:

		MainWindow* _mainWindow;

		bool _isDocumentModified;
		bool _isFileLoaded;
		char _filePath[MAX_PATH];
		char _fileName[MAX_PATH];
		
};

#endif // DOCUMENTMANAGER_H

