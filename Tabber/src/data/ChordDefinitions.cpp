#include "ChordDefinitions.h" 

const char ChordDefinitions::CHORD_DEFINITIONS_FILE_NAME[] = "chords.ini";


ChordDefinitions::ChordDefinitions()
{
	GetAbsoluteFileName(_chordDefinitionsFileFullName, CHORD_DEFINITIONS_FILE_NAME);	
	OBJECT_CREATED;
}


ChordDefinitions::~ChordDefinitions()
{
	OBJECT_DELETED;
}


/**
 * Basically, a ini file parser. Task is complicated because I do not kow the
 * names of the sections nor the name of the keys -- and also because ini
 * parsing functions in the API are a pain in the ass.
 */
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
	}
	else //ini file is empty
	{
		loadDefaultValues();
		save(); //save values to create a correct ini file
	}
	delete parser;
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
		writeprivateprofile*s do not erase previous values */
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
		throw new RuntimeException("ChordDefinitions::save", "Could not write chords into file");

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
		throw new RuntimeException("ChordDefinitions::save", "Could not open chords definitions file");
	}
	
	CloseHandle(hFile);
}


int ChordDefinitions::getChordGroupCount() const
{
	return DynamicPointerArray<ChordGroup>::getElementCount();
}


ChordGroup* ChordDefinitions::getChordGroupAt(int index)
{
	return DynamicPointerArray<ChordGroup>::getElementAt(index);
}


void ChordDefinitions::addChordGroup(ChordGroup* newGroup)
{
	DynamicPointerArray<ChordGroup>::addElement(newGroup);
}

