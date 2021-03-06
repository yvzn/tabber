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

#ifndef GUITARCHORD_H
#define GUITARCHORD_H

#include "../util/utilities.h"


class GuitarChord
{

	public:

		GuitarChord(const char* , const char* );
		virtual ~GuitarChord();
		
		inline const char* getName()      const { return _name;      }
		inline int         getNoteCount() const { return _noteCount; }
		
		inline const char* getNote  (int n)  const { assert(n<_noteCount); return (_notes[n][0] == INVISIBLE_NOTE) ? NULL : _notes[n] ; }
		       const char* getNotes ()       const;

		int getWidth() const { return _width; }

	public:

		static const char EXTENDED_SEQUENCE_SEPARATOR;
		static const char INVISIBLE_NOTE;

	protected:

		void createExtendedChord(const char* );

	protected:

		char*   _name;
		char**  _notes;
		int     _noteCount;
		bool    _isExtended;
		int     _width;      //< character count of the largest note in the tuning

		static char conversionBuffer[40];

};


#endif // GUITARCHORD_H

