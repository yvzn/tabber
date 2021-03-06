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

#ifndef EDITIONMODES_H
#define EDITIONMODES_H

/**
 * Builds conversion functions from edition modes to command identifiers.
 * The first mode value is binded to the first command identifier, therefore conversions just consist in additions/substractions.
 */
#define DECLARE_CONVERSION_FUNCTIONS(EditionMode, FirstCommandId)             \
inline unsigned int GetCommandId (EditionMode mode)                           \
    { return (unsigned int)(mode + FirstCommandId); }                         \
inline EditionMode Get##EditionMode(unsigned int commandId)                   \
    { return (EditionMode)  (commandId - FirstCommandId); }


typedef enum { INSERT=0, OVERWRITE, SPECIAL, TYPING_MODE_COUNT } TypingMode;

// declares GetCommandId(TypingMode ) and GetTypingMode(unsigned int )
DECLARE_CONVERSION_FUNCTIONS(TypingMode, ID_OPTIONS_TYPING_INSERT);


/**
 * Index of the current guitar tuning inside the tuning definitions list
 */
typedef unsigned int TuningIndex;

// declares GetCommandId(TuningIndex ) and GetTuningIndex(unsigned int )
DECLARE_CONVERSION_FUNCTIONS(TuningIndex, ID_OPTIONS_TUNINGS_NONE);



/**
 * Modifies the way chords are added
 */
typedef enum { ADD_EXTRA_SPACE=0, ADD_NAME, CHORD_MODE_COUNT } ChordMode;

// declares GetCommandId(ChordMode ) and GetChordMode(unsigned int )
DECLARE_CONVERSION_FUNCTIONS(ChordMode, ID_OPTIONS_CHORD_EXTRA_SPACE);



/**
 * Index of the current chord to be added inside staff
 */
typedef struct
{
	int group;
	int chord;
}
ChordIndex;


/**
 * Conversion functions for chord indexes.
 * Windows's command id's are stored in a WORD (16 bits). I use the highest bit
 * to mark chords commandIds (1<<15 = 32768 = IDC_FIRST_CHORD), the seven following
 * bits to code chord's groupIndex, and the lower 8 bits to code chord's Id.
 */
inline unsigned int GetCommandId (ChordIndex index)
{
	return IDC_FIRST_CHORD + ((index.group & 0x7F) << 8) + (index.chord & 0xFF);
}


inline ChordIndex GetChordIndex(unsigned int commandId)
{
	assert( commandId >= IDC_FIRST_CHORD);
	commandId -= IDC_FIRST_CHORD;

	ChordIndex result = { (commandId >> 8) & 0x7F , commandId & 0xFF };
	return result;
}


#endif //EDITIONMODES_H
