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
		void onDocumentOpen   ( );
		void onDocumentOpen   (const char* );

	protected:

		void blankDocumentFlags();
		void updateMainWindowTitle();
		void updateFileName();
		void loadSpecifiedDocument();
	
	protected:

		MainWindow* _mainWindow;

		bool _isDocumentModified;
		bool _isFileLoaded;
		char _filePathAndName[MAX_PATH];
		char* _fileName;
		
		OPENFILENAME _fileDialogOptions;
		
};

#endif // DOCUMENTINTERFACE_H

