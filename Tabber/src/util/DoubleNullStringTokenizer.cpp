#include "DoubleNullStringTokenizer.h"


/**
 * @param doubleNullString a pointer to one or more null-terminated strings, concatenated. The last string is followed by a second null character (hence the double-null).
 * @param doubleNullStringLength sum of strings' length, including the null characters
 * @warning the double string to be tokenized must remain in memory during parsing
 */
DoubleNullStringTokenizer::DoubleNullStringTokenizer(
	const char* doubleNullString,
 	int doubleNullStringLength)
{
	assert(doubleNullString != NULL);

	_string = doubleNullString;
	_currentToken = const_cast<char*>(_string); //I just need the address
	_endOfString = _currentToken + (doubleNullStringLength-1) * sizeof(char ); //address of the very last '\0'
	
	OBJECT_CREATED;
}


DoubleNullStringTokenizer::~DoubleNullStringTokenizer()
{
	OBJECT_DELETED;
}


bool DoubleNullStringTokenizer::hasMoreTokens() const
{
	return _currentToken < _endOfString;
}


/**
 * move the internal pointer and return the next token
 */
const char* DoubleNullStringTokenizer::nextToken()
{
	const char* result=_currentToken;
	
	int tokenLength = lstrlen(_currentToken);
	_currentToken += (1+tokenLength) * sizeof(char ); //+1 because lstrlen does not include trailing '\0'
	
	return result;
}

