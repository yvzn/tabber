#include "ApplicationSettings.h"

const char ApplicationSettings::APPLICATION_SETTINGS_FILE_NAME[] = "tabber.ini";


ApplicationSettings::ApplicationSettings()
{
	GetAbsoluteFileName(_settingsFileFullName, APPLICATION_SETTINGS_FILE_NAME);
	OBJECT_CREATED;
}


ApplicationSettings::~ApplicationSettings()
{
	OBJECT_DELETED;
}


void ApplicationSettings::load()
{
	_mainWindowRect.top       = GetPrivateProfileInt("window", "top",    -1, _settingsFileFullName);
	_mainWindowRect.right     = GetPrivateProfileInt("window", "right",  -1, _settingsFileFullName);
	_mainWindowRect.bottom    = GetPrivateProfileInt("window", "bottom",   -1, _settingsFileFullName);
	_mainWindowRect.left      = GetPrivateProfileInt("window", "left", -1, _settingsFileFullName);
	
	_mainWindowMaximizedState = GetPrivateProfileInt("window", "maximized", -1, _settingsFileFullName) > 0;
}


void ApplicationSettings::save() const
{
	char* conversionBuffer = new char[20];

	wsprintf(conversionBuffer, "%d", _mainWindowRect.top)    ; WritePrivateProfileString("window", "top",    conversionBuffer, _settingsFileFullName);
	wsprintf(conversionBuffer, "%d", _mainWindowRect.right)  ; WritePrivateProfileString("window", "right",  conversionBuffer, _settingsFileFullName);
	wsprintf(conversionBuffer, "%d", _mainWindowRect.bottom) ; WritePrivateProfileString("window", "bottom", conversionBuffer, _settingsFileFullName);
	wsprintf(conversionBuffer, "%d", _mainWindowRect.left)   ; WritePrivateProfileString("window", "left",   conversionBuffer, _settingsFileFullName);
	
	wsprintf(conversionBuffer, "%d", _mainWindowMaximizedState ? 1 : 0); WritePrivateProfileString("window", "maximized", conversionBuffer, _settingsFileFullName);
	
	delete [] conversionBuffer;
}


const RECT& ApplicationSettings::getMainWindowRect() const
{
	return _mainWindowRect;
}

void ApplicationSettings::setMainWindowRect(const RECT& newRect)
{
	CopyMemory(&_mainWindowRect, &newRect, sizeof(RECT));
}


bool ApplicationSettings::getMainWindowMaximizedState() const
{
	return _mainWindowMaximizedState;
}


void ApplicationSettings::setMainWindowMaximizedState(bool newMaximizedState)
{
	_mainWindowMaximizedState = newMaximizedState;
}

