/*
 * Tabber - Guitar Tablature Editor, freeware and open-source
 * Copyright (C) 2004  Yvan Razafindramanana
 *
 * Additionnal Contributors:
 * <place your name here :)>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ApplicationSettings.h"

const char ApplicationSettings::APPLICATION_SETTINGS_FILE_NAME[] = "tabber.ini";


ApplicationSettings::ApplicationSettings()
{
	System::getAbsoluteFileName(_settingsFileFullName, APPLICATION_SETTINGS_FILE_NAME);
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
	
	_staffWidth = GetPrivateProfileInt("staff", "width", 80, _settingsFileFullName);
	_chordDepth = GetPrivateProfileInt("staff", "height", 6, _settingsFileFullName);
}


void ApplicationSettings::save() const
{
	WritePrivateProfileString("window", "top",     String::toString(_mainWindowRect.top)    , _settingsFileFullName);
	WritePrivateProfileString("window", "right",   String::toString(_mainWindowRect.right)  , _settingsFileFullName);
	WritePrivateProfileString("window", "bottom",  String::toString(_mainWindowRect.bottom) , _settingsFileFullName);
	WritePrivateProfileString("window", "left",    String::toString(_mainWindowRect.left)   , _settingsFileFullName);
	
	WritePrivateProfileString("window", "maximized", String::toString(_isMainWindowMaximized ? 1 : 0), _settingsFileFullName);
	
	WritePrivateProfileString("font", "size",   String::toString(_editAreaFont.lfHeight) , _settingsFileFullName);
	WritePrivateProfileString("font", "weight", String::toString(_editAreaFont.lfWeight) , _settingsFileFullName);
	WritePrivateProfileString("font", "italic", String::toString(_editAreaFont.lfItalic) , _settingsFileFullName);
	WritePrivateProfileString("font", "face", _editAreaFont.lfFaceName, _settingsFileFullName);

	WritePrivateProfileString("edition", "typing", String::toString(_typingMode) , _settingsFileFullName);
	WritePrivateProfileString("edition", "tuning", String::toString(_tuningIndex)  , _settingsFileFullName);
	
	WritePrivateProfileString("edition", "extra-space", String::toString(_isChordModeEnabled[ADD_EXTRA_SPACE]), _settingsFileFullName);
	WritePrivateProfileString("edition", "add-name", String::toString(_isChordModeEnabled[ADD_NAME]), _settingsFileFullName);
	
	WritePrivateProfileString("staff", "width", String::toString(_staffWidth), _settingsFileFullName);
	WritePrivateProfileString("staff", "height", String::toString(_chordDepth), _settingsFileFullName);
}



