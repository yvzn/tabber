#include "ChordDefinitions.h" 

const char ChordDefinitions::CHORD_DEFINITIONS_FILE_NAME[] = "chords.ini";


ChordDefinitions::ChordDefinitions()
{
	GetAbsoluteFileName(_chordDefinitionsFileFullName, CHORD_DEFINITIONS_FILE_NAME);
	_chords = new DynamicPointerArray<GuitarChord>();
	_chordGroups = new DynamicPointerArray<ChordGroup>();
	
	OBJECT_CREATED;
}


ChordDefinitions::~ChordDefinitions()
{
	delete _chords;
	delete _chordGroups;
	OBJECT_DELETED;
}


/**
 * Basically, a ini file parser. Task is complicated because I do not kow the
 * names of the sections nor the name of the keys -- and also because ini
 * parsing functions in the API are a pain in the ass.
 */
void ChordDefinitions::load()
{
	assert(_chords != NULL && _chordGroups != NULL);

	IniFileSequentialParser* parser = new IniFileSequentialParser(_chordDefinitionsFileFullName);
	if(parser->hasMoreSections())
	{
		while(parser->hasMoreSections())
		{
			const char* section = parser->nextSection();
			int keyCount = 0;
  
   			while(parser->sectionHasMoreKeys())
	     	{
	     		const char* const* key=parser->nextKey();
	     		_chords->addElement(new GuitarChord(key[0], key[1]));
	     		++keyCount;
	       	}
       	
	       	_chordGroups->addElement(new ChordGroup(section, keyCount));
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
	assert(_chords != NULL && _chordGroups != NULL);

	_chords->addElement(new GuitarChord("A", "x02220"));
	_chords->addElement(new GuitarChord("B", "x13331"));
	_chords->addElement(new GuitarChord("C", "x32010"));
	_chords->addElement(new GuitarChord("D", "xx0232"));
	_chords->addElement(new GuitarChord("E", "022100"));
	_chords->addElement(new GuitarChord("F", "133211"));
	_chords->addElement(new GuitarChord("G", "320003"));

	_chordGroups->addElement(new ChordGroup("Default", _chords->getElementCount())); 
}


void ChordDefinitions::save() const
{
	assert(
 		_chords != NULL &&
		_chordGroups != NULL &&
 		_chordGroups->getElementCount() > 0 );

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

#define wf(str) WriteFile(hFile, str, (DWORD)strlen(str), &bytesWritten, NULL)

	if(hFile != INVALID_HANDLE_VALUE)
	{
		int groupCount = _chordGroups->getElementCount();
		int chordCount = _chords->getElementCount();
	
		for(int groupIndex=0, chordIndex=0; groupIndex < groupCount; ++groupIndex )
		{
			int groupElementCount = _chordGroups->getElementAt(groupIndex)->getElementCount();
			const char* groupName = _chordGroups->getElementAt(groupIndex)->getName();
			wf("[");
			wf(groupName);
			wf("]\r\n");
		
			for(int maxChordIndex=chordIndex+groupElementCount; chordIndex<maxChordIndex && chordIndex<chordCount; ++chordIndex)
			{
				const char* chordName = _chords->getElementAt(chordIndex)->getName();
				const char* chordNotes = _chords->getElementAt(chordIndex)->getNotes();
				wf(chordName);
				wf("=");
				wf(chordNotes);
				wf("\r\n");
			}
			
			wf("\r\n");
		}
	}
	else
	{
		throw new RuntimeException("ChordDefinitions::save", "Could not save chords into file");
	}
	
	CloseHandle(hFile);
}

