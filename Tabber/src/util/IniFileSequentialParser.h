#ifndef INIFILESEQUENTIALPARSER_CPP
#define INIFILESEQUENTIALPARSER_CPP

#include "../util/utilities.h"
#include "../util/DoubleNullStringTokenizer.h"


/**
 * A generic tool to parse ini files sequentially, mainly beacuse
 * WinAPI parsing functions are a real pain in the ass. Not suitable for back
 * and forth parsing nor random access (in these cases, you would better use
 * GetPrivateProfile* )
 */
class IniFileSequentialParser
{

	public:

		IniFileSequentialParser(const char* );
		~IniFileSequentialParser();
		
		bool hasMoreSections();
		const char* nextSection();

		bool sectionHasMoreKeys();
		const char* const* nextKey();
		
	protected:
	
		static const int DEFAULT_BUFFER_SIZE;

		char* _IniFileName;
		
		DoubleNullStringTokenizer* _sections;
		DoubleNullStringTokenizer* _keysValuesPairs;
		
	private:
	
		int   _namesBufferSize;
		char* _namesBuffer;
		int   _namesBufferCharactersCount;

		int   _sectionBufferSize;
		char* _sectionBuffer;
		int   _sectionBufferCharactersCount;
		
		char** _keyValueBufferArray;
		
};

#endif // INIFILESEQUENTIALPARSER_CPP

