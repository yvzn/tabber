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
typedef enum { ADD_EXTRA_SPACE=0, ADD_NAME, ARPEGGIO, CHORD_MODE_COUNT } ChordMode;

// declares GetCommandId(ChordMode ) and GetChordMode(unsigned int )
DECLARE_CONVERSION_FUNCTIONS(ChordMode, ID_OPTIONS_CHORD_EXTRA_SPACE);



// TODO: GetChordIndex(unsigned int command Id)


#endif //EDITIONMODES_H
