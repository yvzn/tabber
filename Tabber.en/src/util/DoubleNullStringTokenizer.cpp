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

