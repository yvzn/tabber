#include "Application.h"
#include "../ui/MainWindow.h"


Application::Application()
{
	_settings    = new ApplicationSettings();
	_chords      = new ChordDefinitions();
	_mainWindow  = new MainWindow(this);
	OBJECT_CREATED;
}


Application::~Application()
{
	delete _mainWindow;
	delete _chords;
	delete _settings;
	OBJECT_DELETED;
}


void Application::create(HINSTANCE hApplicationInstance)
{	
	assert(
 		_settings != NULL &&
 		_chords != NULL &&
   		_mainWindow != NULL);

	try
	{
		_settings->load();
		_chords->load();
		
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

