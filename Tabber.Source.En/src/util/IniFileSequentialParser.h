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

