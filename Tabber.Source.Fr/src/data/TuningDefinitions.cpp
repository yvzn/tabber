/*
 * Tabber - Guitar Tablature Editor, freeware and open-source
 * Copyright (C) 2004  Yvan Razafindramanana
 *
 * Additionnal Contributors:
 * <place your name here :)>
 *
 * This file is part of Tabber.
 * 
 * Tabber is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tabber is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tabber.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "TuningDefinitions.h"

const char TuningDefinitions::TUNING_DEFINITIONS_FILE_NAME[] = "tunings.ini";


TuningDefinitions::TuningDefinitions()
{
	System::getAbsoluteFileName(_tuningDefinitionsFileFullName, TUNING_DEFINITIONS_FILE_NAME);
	OBJECT_CREATED;
}


TuningDefinitions::~TuningDefinitions()
{
	OBJECT_DELETED;
}


/**
 * @see ChordDefinitions::load
 */
void TuningDefinitions::load()
{
	IniFileSequentialParser* parser = new IniFileSequentialParser(_tuningDefinitionsFileFullName);
	if(parser->hasMoreSections())
	{
		//treat every definition of every secion as a tuning
		while(parser->hasMoreSections())
		{
			parser->nextSection();
			while(parser->sectionHasMoreKeys())
			{
				const char* const* key=parser->nextKey();
				addElement(new GuitarTuning(key[0], key[1]));
			}
		}

		delete parser;
	}
	else //ini file is empty
	{
		delete parser;

		//load a single default value
		addElement(new GuitarTuning("E", "EADGBe"));
		
		save(); //save values to create a correct ini file
	}
}


/**
 * @see ChordDefinitions::save
 */
void TuningDefinitions::save()
{
	DWORD bytesWritten;

	HANDLE hFile = CreateFile(
		_tuningDefinitionsFileFullName,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		CREATE_ALWAYS,
		0,
		NULL );

#define wf(str) \
	if(!WriteFile(hFile, str, (DWORD)strlen(str), &bytesWritten, NULL)) \
	throw new RuntimeException("TuningDefinitions::save", System::getLocaleString(IDERR_SAVE_TUNINGS));

	if(hFile != INVALID_HANDLE_VALUE)
	{
		wf("[Tunings]\r\n");

		int tuningCount = getTuningCount();
		for(int tuningIndex=0; tuningIndex<tuningCount; ++tuningIndex) //tuning 0 is not saved
		{
			GuitarTuning* currentTuning = getTuningAt(tuningIndex);
			wf(currentTuning->getName());
			wf("=");
			wf(currentTuning->getNotes());
			wf("\r\n");
		}
	}
	else
	{
		throw new RuntimeException("TuningDefinitions::save", System::getLocaleString(IDERR_OPEN_TUNINGS_FILE));
	}

#undef wf

	CloseHandle(hFile);
}
