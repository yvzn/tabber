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
		char* buffer = new char[length+1];
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
§* Filters messages dedicated to specific child windows (keyboard shortcuts, dialog messages, ...)
§* @param message system message to translate
§* @return true if message is intercepted somewhere inside the application
§*/
bool Application::interceptsMessage(MSG* message)
{
	return
		TranslateAccelerator(
			_mainWindow->getWindowHandle(),
			_accelerators,
			message ) == TRUE
		||
		_mainWindow->interceptsMessage(message);
}

