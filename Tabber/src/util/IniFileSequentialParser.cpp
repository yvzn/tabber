#include "IniFileSequentialParser.h" 

const int IniFileSequentialParser::DEFAULT_BUFFER_SIZE = 4; // >=4 or else I cannot detect GetPrivateProfileSectionNames failure


/**
 * @param fileName <strong>absolute</strong> file name of the ini file
 */
IniFileSequentialParser::IniFileSequentialParser(const char* fileName)
{
	_IniFileName = new char[lstrlen(fileName)+1];
	strcpy(_IniFileName, fileName);

	//buffers initialisation
	_namesBufferSize = DEFAULT_BUFFER_SIZE;
	_namesBuffer = new char[_namesBufferSize];
	_sectionBufferSize = DEFAULT_BUFFER_SIZE;
	_sectionBuffer = new char[_sectionBufferSize];
	_keyValueBufferArray = NULL;
	_keysValuesPairs = NULL;

	// read section names
	for(bool bufferIsNotLargeEnough=true; bufferIsNotLargeEnough; )
	{
		_namesBufferCharactersCount =
  			GetPrivateProfileSectionNames(_namesBuffer, _namesBufferSize, _IniFileName);
		
 		if(_namesBufferCharactersCount != 0 && _namesBufferCharactersCount >= _namesBufferSize-2)
 		{
 			//not large enough buffer: see reference for GetPrivateProfileSectionNames 
			delete [] _namesBuffer;
			_namesBufferSize *= 2;
			_namesBuffer = new char[_namesBufferSize];
		}
		else
		{
 			bufferIsNotLargeEnough = false;
		}
 	}

	if(_namesBufferCharactersCount > 0) //one or more sections present in file
	{
		_sections = new DoubleNullStringTokenizer(_namesBuffer, _namesBufferCharactersCount);
	}
	else
	{
		_sections = NULL;
	}

	OBJECT_CREATED;
}


IniFileSequentialParser::~IniFileSequentialParser()
{
	if(_sections != NULL) delete _sections;
	if(_keysValuesPairs != NULL) delete _keysValuesPairs;

	if(_keyValueBufferArray != NULL)
 	{
  		delete [] _keyValueBufferArray[0];
  		delete [] _keyValueBufferArray[1];
  		delete [] _keyValueBufferArray;
	}
	
	delete [] _sectionBuffer;
	delete [] _namesBuffer;
	delete [] _IniFileName;

	OBJECT_DELETED;
}


bool IniFileSequentialParser::hasMoreSections()
{
	if(_sections == NULL)
		return false;
	else
		return _sections->hasMoreTokens();
}


const char* IniFileSequentialParser::nextSection()
{
	assert(hasMoreSections());

	const char* section = _sections->nextToken();
	
	// read current section values
	for(bool bufferIsNotLargeEnough=true; bufferIsNotLargeEnough; )
	{
		_sectionBufferCharactersCount =
			GetPrivateProfileSection(section, _sectionBuffer, _sectionBufferSize, _IniFileName);

 		if(_sectionBufferCharactersCount != 0 && _sectionBufferCharactersCount >= _sectionBufferSize-2)
 		{
 			delete [] _sectionBuffer;
			_sectionBufferSize *= 2;
			_sectionBuffer = new char[_sectionBufferSize];
		}
		else
		{
 			bufferIsNotLargeEnough = false;
 		}
	}
 			
	if(_sectionBufferCharactersCount > 0) //one or more keys present in section
	{
		if(_keysValuesPairs != NULL) delete _keysValuesPairs;
		_keysValuesPairs = new DoubleNullStringTokenizer(_sectionBuffer, _sectionBufferCharactersCount);
	}
	else
	{
		_keysValuesPairs = NULL;
	}
	
	return section;
}


bool IniFileSequentialParser::sectionHasMoreKeys()
{
	if(_keysValuesPairs == NULL)
		return false;
	else
		return _keysValuesPairs->hasMoreTokens();
}


/**
 * @return a array with the next key name at position[0] and the next key value at [1]
 */
const char* const* IniFileSequentialParser::nextKey()
{
	assert(sectionHasMoreKeys());
	
	const char* pair = _keysValuesPairs->nextToken();
	
	// pairLength can be used as a majoring size for key & value buffers
 	// as pair = key + '=' + value (the '=' saves the extra
  	// allocation for trailing '\0'
	int pairLength = lstrlen(pair);

	// multi-dimensional array allocation "a la mano" because C++ does not
	// allow variable length multi-dimentional arrays
	if(_keyValueBufferArray != NULL)
 	{
  		delete [] _keyValueBufferArray[0];
  		delete [] _keyValueBufferArray[1];
  		delete [] _keyValueBufferArray;
	}
 	_keyValueBufferArray = new char*[2];
	_keyValueBufferArray[0] = new char[pairLength];
	_keyValueBufferArray[1] = new char[pairLength];
	
	int pos;
	for(pos=0 ; pair[pos] != '=' && pos < pairLength; pos++)
	{
		_keyValueBufferArray[0][pos] = pair[pos];
	}
	_keyValueBufferArray[0][pos] = '\0';
	
	pos++; //pass the '=' sign
	
	int valuePos = pos;
	for( ; pos < pairLength; pos++)
	{
		_keyValueBufferArray[1][pos-valuePos] = pair[pos];
	}
	_keyValueBufferArray[1][pos-valuePos] = '\0';
	
	return _keyValueBufferArray;
}




