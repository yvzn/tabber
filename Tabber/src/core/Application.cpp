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
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("Application::create", ex);
	}
}


void Application::show(int showState) const
{
	assert(_mainWindow != NULL);
	_mainWindow->show(showState);
}


ApplicationSettings* Application::getSettings()
{
	return _settings;
}


ChordDefinitions* Application::getChordDefinitions()
{
	return _chords;
}

