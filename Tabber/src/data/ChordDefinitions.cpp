#include "ChordDefinitions.h" 

const char CChordDefinitions::SZ_CHORD_DEFINITIONS_FILE_NAME[] = "chords.ini";


CChordDefinitions::CChordDefinitions()
{
	GetAbsoluteFileName(m_szAbsoluteFileName, SZ_CHORD_DEFINITIONS_FILE_NAME);
	m_lpChords = new CDynamicArray<CChord>();
	m_lpChordGroups = new CDynamicArray<CChordGroup>();
	
	OBJECT_CREATED;
}


CChordDefinitions::~CChordDefinitions()
{
	delete m_lpChords;
	delete m_lpChordGroups;
	OBJECT_DELETED;
}


void CChordDefinitions::load()
{
	assert(m_lpChords != NULL && m_lpChordGroups != NULL);
	char line[256];
  
	std::ifstream chordFile (m_szAbsoluteFileName, std::ios::in);
	if(chordFile.good())
	{
		chordFile.getline(line);
		switch(line[0])
		{
			case ';': case '\0';
			{
				//comment, empty lines : non significative
				break;
			}
			case '[':
			{
				//section start : new chord group
				break;
			}
			default
			{
				//key=value pair : new chord
			}	
		}
	}
	else
	{
		loadDefaultValues();
	}
	
	chordFile.close();
}


void CChordDefinitions::loadDefaultValues()
{
	assert(m_lpChords != NULL && m_lpChordGroups != NULL);

	CChordGroup* lpChordGroup = new CChordGroup("Default");
	lpChordGroup->setElementCount(7);
	m_lpChordGroups->addElement(lpChordGroup);
	
	m_lpChords->addElement(new CChord("A", "x02220"));
	m_lpChords->addElement(new CChord("B", "x13331"));
	m_lpChords->addElement(new CChord("C", "x32010"));
	m_lpChords->addElement(new CChord("D", "xx0232"));
	m_lpChords->addElement(new CChord("E", "022100"));
	m_lpChords->addElement(new CChord("F", "133211"));
	m_lpChords->addElement(new CChord("G", "320003"));
}


void CChordDefinitions::save()
{
	assert(
 		m_lpChords != NULL &&
		m_lpChordGroups != NULL &&
 		m_lpChordGroups->getElementCount() > 0 );

	LPCSTR szGroupName;
	int nGroupCount=m_lpChordGroups->getElementCount();
	int nChordCount=m_lpChords->getElementCount();
	
	for(int nGroupIndex=0, nChordIndex=0; nGroupIndex < nGroupCount; ++nGroupIndex )
	{
		int nGroupElementCount = m_lpChordGroups->getElementAt(nGroupIndex)->getElementCount();
		szGroupName = m_lpChordGroups->getElementAt(nGroupIndex)->getName();
		
		for(int nCount=nChordIndex+nGroupElementCount; nChordIndex<nCount && nChordIndex<nChordCount; ++nChordIndex)
		{
			WritePrivateProfileString(
  				szGroupName,
	     		m_lpChords->getElementAt(nChordIndex)->getName(),
	       		m_lpChords->getElementAt(nChordIndex)->getNotes(),
	         	m_szAbsoluteFileName );
		}
	}
}

