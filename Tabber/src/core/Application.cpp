#include "Application.h"


// required because CMainWindow::WindowProc must be static
#include "../ui/MainWindow.h"
CMainWindow* g_lpMainWindow; 


CApplication::CApplication()
{
	m_lpSettings   = new CGlobalSettings();
	m_lpChords     = new CChordDefinitions();
	g_lpMainWindow = new CMainWindow(this);
	OBJECT_CREATED;
}


CApplication::~CApplication()
{
	delete g_lpMainWindow;
	delete m_lpChords;
	delete m_lpSettings;
	OBJECT_DELETED;
}


void CApplication::create(HINSTANCE hApplicationInstance)
{	
	assert(
 		m_lpSettings != NULL &&
 		m_lpChords != NULL &&
   		g_lpMainWindow != NULL);

	try
	{
		m_lpSettings->load();
		m_lpChords->load();
		
		g_lpMainWindow->create(
  			hApplicationInstance,
  			m_lpSettings->getMainWindowX(),
  			m_lpSettings->getMainWindowY(),
  			m_lpSettings->getMainWindowWidth(),
  			m_lpSettings->getMainWindowHeight()
          	);
	}
	catch(CRuntimeException* lpException)
	{
		throw new CRuntimeException("CApplication::create", lpException);
	}
}


void CApplication::show(int nWindowShowState)
{
	assert(g_lpMainWindow != NULL);
	g_lpMainWindow->show(nWindowShowState);
}


CGlobalSettings* CApplication::getGlobalSettings()
{
	return m_lpSettings;
}


CChordDefinitions* CApplication::getChordDefinitions()
{
	return m_lpChords;
}

