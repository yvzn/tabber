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
	_mainWindowRect.bottom    = GetPrivateProfileInt("window", "bottom", -1, _settingsFileFullName);
	_mainWindowRect.left      = GetPrivateProfileInt("window", "left",   -1, _settingsFileFullName);
	
	_mainWindowMaximizedState = GetPrivateProfileInt("window", "maximized", 0, _settingsFileFullName) > 0;

	ZeroMemory(&_editAreaFont, sizeof(_editAreaFont));
	_editAreaFont.lfHeight = GetPrivateProfileInt("window", "font-size", 0, _settingsFileFullName);
	_editAreaFont.lfWeight = GetPrivateProfileInt("window", "font-weight", 0, _settingsFileFullName);
	_editAreaFont.lfItalic = GetPrivateProfileInt("window", "font-italic", 0, _settingsFileFullName);
	GetPrivateProfileString("window", "font-family", "Courier New", _editAreaFont.lfFaceName, LF_FACESIZE, _settingsFileFullName);
}


void ApplicationSettings::save() const
{
	char* conversionBuffer = new char[20];

	wsprintf(conversionBuffer, "%d", _mainWindowRect.top)    ; WritePrivateProfileString("window", "top",    conversionBuffer, _settingsFileFullName);
	wsprintf(conversionBuffer, "%d", _mainWindowRect.right)  ; WritePrivateProfileString("window", "right",  conversionBuffer, _settingsFileFullName);
	wsprintf(conversionBuffer, "%d", _mainWindowRect.bottom) ; WritePrivateProfileString("window", "bottom", conversionBuffer, _settingsFileFullName);
	wsprintf(conversionBuffer, "%d", _mainWindowRect.left)   ; WritePrivateProfileString("window", "left",   conversionBuffer, _settingsFileFullName);
	
	wsprintf(conversionBuffer, "%d", _mainWindowMaximizedState ? 1 : 0); WritePrivateProfileString("window", "maximized", conversionBuffer, _settingsFileFullName);
	
	wsprintf(conversionBuffer, "%d", _editAreaFont.lfHeight); WritePrivateProfileString("window", "font-size", conversionBuffer, _settingsFileFullName);
	wsprintf(conversionBuffer, "%d", _editAreaFont.lfWeight); WritePrivateProfileString("window", "font-weight", conversionBuffer, _settingsFileFullName);
	wsprintf(conversionBuffer, "%d", _editAreaFont.lfItalic); WritePrivateProfileString("window", "font-italic", conversionBuffer, _settingsFileFullName);
	WritePrivateProfileString("window", "font-family", _editAreaFont.lfFaceName, _settingsFileFullName);

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


const LOGFONT& ApplicationSettings::getEditAreaFont() const
{
	return _editAreaFont;
}


void ApplicationSettings::setEditAreaFont(const LOGFONT& newValue)
{
	CopyMemory(&_editAreaFont, &newValue, sizeof(_editAreaFont));
}


