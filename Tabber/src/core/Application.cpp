#include "Application.h"


// required because MainWindow::WindowProc must be static
#include "../ui/MainWindow.h"
MainWindow* gMainWindow; 


Application::Application()
{
	_settings    = new ApplicationSettings();
	_chords      = new ChordDefinitions();
	gMainWindow  = new MainWindow(this);
	OBJECT_CREATED;
}


Application::~Application()
{
	delete gMainWindow;
	delete _chords;
	delete _settings;
	OBJECT_DELETED;
}


void Application::create(HINSTANCE hApplicationInstance)
{	
	assert(
 		_settings != NULL &&
 		_chords != NULL &&
   		gMainWindow != NULL);

	try
	{
		_settings->load();
		_chords->load();
		
		gMainWindow->create(hApplicationInstance);
	}
	catch(RuntimeException* ex)
	{
		throw new RuntimeException("Application::create", ex);
	}
}


void Application::show(int showState) const
{
	assert(gMainWindow != NULL);
	gMainWindow->show(showState);
}


ApplicationSettings* Application::getSettings()
{
	return _settings;
}


ChordDefinitions* Application::getChordDefinitions()
{
	return _chords;
}

