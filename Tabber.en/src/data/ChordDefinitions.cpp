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

#include "ChordDefinitions.h" 

const char ChordDefinitions::CHORD_DEFINITIONS_FILE_NAME[] = "chords.ini";
const int  ChordDefinitions::MAX_GROUP_COUNT = 128;


ChordDefinitions::ChordDefinitions()
{
	System::getAbsoluteFileName(_chordDefinitionsFileFullName, CHORD_DEFINITIONS_FILE_NAME);	
	OBJECT_CREATED;
}


ChordDefinitions::~ChordDefinitions()
{
	OBJECT_DELETED;
}


void ChordDefinitions::load()
{
	IniFileSequentialParser* parser = new IniFileSequentialParser(_chordDefinitionsFileFullName);
	if(parser->hasMoreSections())
	{
		while(parser->hasMoreSections())
		{
			const char* section = parser->nextSection();
			
			ChordGroup* currentGroup = new ChordGroup(section);
  
   			while(parser->sectionHasMoreKeys())
	     	{
	     		const char* const* key=parser->nextKey();
	     		currentGroup->addChord(new GuitarChord(key[0], key[1]));
	       	}
       	
			addChordGroup(currentGroup);
		}

		delete parser;
	}
	else //ini file is empty
	{
		delete parser;

		loadDefaultValues();
		save(); //save values to create a correct ini file
	}
}


void ChordDefinitions::loadDefaultValues()
{
	ChordGroup* defaultGroup = new ChordGroup("Default");
	{
		defaultGroup->addChord(new GuitarChord("A", "x02220"));
		defaultGroup->addChord(new GuitarChord("B", "x13331"));
		defaultGroup->addChord(new GuitarChord("C", "x32010"));
		defaultGroup->addChord(new GuitarChord("D", "xx0232"));
		defaultGroup->addChord(new GuitarChord("E", "022100"));
		defaultGroup->addChord(new GuitarChord("F", "133211"));
 		defaultGroup->addChord(new GuitarChord("G", "320003"));
	}
	addChordGroup(defaultGroup); 
}


void ChordDefinitions::save()
{
	assert(getChordGroupCount() > 0 );

	DWORD bytesWritten; //useless but required

	/* I write ini file on disk manually instead of using API functions because
		writeprivateprofile*s do not take structure reorganisation in account */
	HANDLE hFile = CreateFile(
		_chordDefinitionsFileFullName,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		CREATE_ALWAYS,
		0,
  		NULL );

#define wf(str) \
	if(!WriteFile(hFile, str, (DWORD)strlen(str), &bytesWritten, NULL)) \
	throw new RuntimeException("ChordDefinitions::save", System::getLocaleString(IDERR_SAVE_CHORDS));

	if(hFile != INVALID_HANDLE_VALUE)
	{
		int groupCount = getChordGroupCount();
		for(int groupIndex=0; groupIndex < groupCount; ++groupIndex )
		{
			ChordGroup* currentGroup = getChordGroupAt(groupIndex);
			wf("[");
			wf(currentGroup->getName());
			wf("]\r\n");
			
			int chordCount = currentGroup->getChordCount();
			for(int chordIndex=0; chordIndex<chordCount; ++chordIndex)
			{
				GuitarChord* currentChord = currentGroup->getChordAt(chordIndex);
				wf(currentChord->getName());
				wf("=");
				wf(currentChord->getNotes());
				wf("\r\n");			
			}
			
			wf("\r\n");
		}
	}
	else
	{
		throw new RuntimeException("ChordDefinitions::save", System::getLocaleString(IDERR_OPEN_CHORDS_FILE));
	}
	
#undef wf
	
	CloseHandle(hFile);
}


void ChordDefinitions::addChordGroup(ChordGroup* newGroup)
{
	if( getChordGroupCount() < MAX_GROUP_COUNT )
	{
		DynamicPointerArray<ChordGroup>::addElement(newGroup);
	}
}

