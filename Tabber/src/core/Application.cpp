#include "Application.h"
#include "../ui/MainWindow.h"


Application::Application()
{
	_settings     = new ApplicationSettings();
	_chords       = new ChordDefinitions();
	_tunings      = new TuningDefinitions();
	_mainWindow   = new MainWindow(this);
	OBJECT_CREATED;
}


Application::~Application()
{
	delete _mainWindow;
	delete _tunings;
	delete _chords;
	delete _settings;
	OBJECT_DELETED;
}


void Application::create(HINSTANCE hApplicationInstance)
{	
	try
	{
		_settings->load();
		_chords->load();
		_tunings->load();
		
		_mainWindow->create(hApplicationInstance);

		_accelerators = LoadAccelerators(hApplicationInstance, MAKEINTRESOURCE(IDR_ACCELERATORS));
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("Application::create", ex);
	}
}


void Application::show(int showState) const
{
	_mainWindow->show(showState);
}


void Application::processCommandLine(const char* commandLine)
{
    int length = lstrlen(commandLine);
    if(length > 0)
    {
	    //try to parse commandLine as a file name and then open it
    	char* buffer = new char[length];
    	char* fileName;
    
    	lstrcpy(buffer, commandLine);
    
    	//Win32's long file names are surrounded by quotes (")
    	if(buffer[0] == '\"')
    	{
	        fileName = &buffer[1];
	        
            //find the other quote and terminate string there
            int endingQuotePos=2;
            while(endingQuotePos<length && buffer[endingQuotePos] != '\"')
            {
                ++endingQuotePos;
    		}
            if(endingQuotePos<length)
            {
    			buffer[endingQuotePos] = '\0';
    		}
    	}
    	else
    	{
    		fileName = buffer;
    	}

    	_mainWindow->getDocumentInterface()->onDocumentOpen(fileName);

    	delete [] buffer;
    }
}


/**
 * Handles (translates and dispatches) keyboard shortcuts
 * @param message system message to translate
 * @return true if message has been correctly identified and handled as a shortcut
 */
bool Application::translateAccelerator(MSG* message)
{
	return TranslateAccelerator(
 		_mainWindow->getWindowHandle(),
   		_accelerators,
   		message );
}



