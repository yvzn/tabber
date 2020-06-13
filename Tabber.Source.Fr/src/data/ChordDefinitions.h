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

#ifndef CHORDDEFINITIONS_H
#define CHORDDEFINITIONS_H

#include "../util/utilities.h"
#include "../util/DynamicPointerArray.h"
#include "../util/IniFileSequentialParser.h"
#include "../data/ChordGroup.h"


class ChordDefinitions: protected DynamicPointerArray<ChordGroup>
{

	public:

		ChordDefinitions();
		~ChordDefinitions();
		
		void load              ();
		void save              ();

		void        addChordGroup      (ChordGroup* ) ;
		
		inline int         getChordGroupCount ()          const { return getElementCount();   }
		inline ChordGroup* getChordGroupAt    (int index)       { return getElementAt(index); }

		inline const char*    getFileName   () const    { return _chordDefinitionsFileFullName; }

	protected:

		void loadDefaultValues ();
		
	private:

		static const char CHORD_DEFINITIONS_FILE_NAME[];
		             char _chordDefinitionsFileFullName[MAX_PATH];

		static const int  MAX_GROUP_COUNT;

};

#endif // CHORDDEFINITIONS_H

