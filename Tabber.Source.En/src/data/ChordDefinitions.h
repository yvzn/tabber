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

