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
	
	_isMainWindowMaximized = GetPrivateProfileInt("window", "maximized", 0, _settingsFileFullName) > 0;

	ZeroMemory(&_editAreaFont, sizeof(_editAreaFont));
	_editAreaFont.lfHeight = GetPrivateProfileInt("font", "size", 0, _settingsFileFullName);
	_editAreaFont.lfWeight = GetPrivateProfileInt("font", "weight", 0, _settingsFileFullName);
	_editAreaFont.lfItalic = GetPrivateProfileInt("font", "italic", 0, _settingsFileFullName);
	GetPrivateProfileString("font", "face", "Courier New", _editAreaFont.lfFaceName, LF_FACESIZE, _settingsFileFullName);
	
	_typingMode = (TypingMode)GetPrivateProfileInt("edition", "typing", 0, _settingsFileFullName);
	_tuningIndex = GetPrivateProfileInt("edition", "tuning", 1, _settingsFileFullName);
	
	_isChordModeEnabled[ADD_EXTRA_SPACE] = GetPrivateProfileInt("edition", "extra-space", 0, _settingsFileFullName) > 0;
	_isChordModeEnabled[ADD_NAME] = GetPrivateProfileInt("edition", "add-name", 0, _settingsFileFullName) > 0;
	_isChordModeEnabled[ARPEGGIO] = GetPrivateProfileInt("edition", "arpeggio", 0, _settingsFileFullName) > 0;
	
	_staffWidth = GetPrivateProfileInt("staff", "width", 80, _settingsFileFullName);
	_chordDepth = GetPrivateProfileInt("staff", "height", 6, _settingsFileFullName);
}


void ApplicationSettings::save() const
{
	WritePrivateProfileString("window", "top",     toString(_mainWindowRect.top)    , _settingsFileFullName);
	WritePrivateProfileString("window", "right",   toString(_mainWindowRect.right)  , _settingsFileFullName);
	WritePrivateProfileString("window", "bottom",  toString(_mainWindowRect.bottom) , _settingsFileFullName);
	WritePrivateProfileString("window", "left",    toString(_mainWindowRect.left)   , _settingsFileFullName);
	
	WritePrivateProfileString("window", "maximized", toString(_isMainWindowMaximized ? 1 : 0), _settingsFileFullName);
	
	WritePrivateProfileString("font", "size",   toString(_editAreaFont.lfHeight) , _settingsFileFullName);
	WritePrivateProfileString("font", "weight", toString(_editAreaFont.lfWeight) , _settingsFileFullName);
	WritePrivateProfileString("font", "italic", toString(_editAreaFont.lfItalic) , _settingsFileFullName);
	WritePrivateProfileString("font", "face", _editAreaFont.lfFaceName, _settingsFileFullName);

	WritePrivateProfileString("edition", "typing", toString(_typingMode) , _settingsFileFullName);
	WritePrivateProfileString("edition", "tuning", toString(_tuningIndex)  , _settingsFileFullName);
	
	WritePrivateProfileString("edition", "extra-space", toString(_isChordModeEnabled[ADD_EXTRA_SPACE]), _settingsFileFullName);
	WritePrivateProfileString("edition", "add-name", toString(_isChordModeEnabled[ADD_NAME]), _settingsFileFullName);
	WritePrivateProfileString("edition", "arpeggio", toString(_isChordModeEnabled[ARPEGGIO]), _settingsFileFullName);
	
	WritePrivateProfileString("staff", "width", toString(_staffWidth), _settingsFileFullName);
	WritePrivateProfileString("staff", "height", toString(_chordDepth), _settingsFileFullName);
}



