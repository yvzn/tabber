#include "GlobalSettings.h"

const char CGlobalSettings::SZ_GLOBAL_SETTINGS_FILE_NAME[] = "tabber.ini";


CGlobalSettings::CGlobalSettings()
{
	GetAbsoluteFileName(m_szCompleteFileName, SZ_GLOBAL_SETTINGS_FILE_NAME);
	OBJECT_CREATED;
}


CGlobalSettings::~CGlobalSettings()
{
	OBJECT_DELETED;
}


void CGlobalSettings::load()
{
	m_cxMainWindowSize = GetPrivateProfileInt("window", "width", CW_USEDEFAULT, m_szCompleteFileName);
	m_cyMainWindowSize = GetPrivateProfileInt("window", "height", CW_USEDEFAULT, m_szCompleteFileName);
	m_xMainWindowPosition = GetPrivateProfileInt("window", "x", CW_USEDEFAULT, m_szCompleteFileName);
	m_yMainWindowPosition = GetPrivateProfileInt("window", "y", CW_USEDEFAULT, m_szCompleteFileName);
}


void CGlobalSettings::save()
{
	LPSTR szBuffer = (LPSTR)GlobalAlloc(GPTR, 20);
	
	wsprintf(szBuffer, "%d", m_cxMainWindowSize); WritePrivateProfileString("window", "width", szBuffer, m_szCompleteFileName);
	wsprintf(szBuffer, "%d", m_cyMainWindowSize); WritePrivateProfileString("window", "height", szBuffer, m_szCompleteFileName);
	wsprintf(szBuffer, "%d", m_xMainWindowPosition); WritePrivateProfileString("window", "x", szBuffer, m_szCompleteFileName);
	wsprintf(szBuffer, "%d", m_yMainWindowPosition); WritePrivateProfileString("window", "y", szBuffer, m_szCompleteFileName);
	
	GlobalFree(szBuffer);
}


int CGlobalSettings::getMainWindowWidth()
{
	return m_cxMainWindowSize;
}


int CGlobalSettings::getMainWindowHeight()
{
	return m_cyMainWindowSize;
}


int CGlobalSettings::getMainWindowX()
{
	return m_xMainWindowPosition;
}


int CGlobalSettings::getMainWindowY()
{
	return m_yMainWindowPosition;
}


void CGlobalSettings::setMainWindowWidth(int cxNewSize)
{
	m_cxMainWindowSize = cxNewSize;
}


void CGlobalSettings::setMainWindowHeight(int cyNewSize)
{
	m_cyMainWindowSize = cyNewSize;
}


void CGlobalSettings::setMainWindowX(int xNewPosition)
{
	m_xMainWindowPosition = xNewPosition;
}


void CGlobalSettings::setMainWindowY(int yNewPosition)
{
	m_yMainWindowPosition = yNewPosition;
}




