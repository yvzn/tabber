#ifndef EDITINGMODES_H
#define EDITINGMODES_H

typedef enum { TYPING_INSERT=0, TYPING_OVERWRITE, TYPING_SPECIAL, TYPING_MODE_COUNT } TypingMode;

inline unsigned int GetCommandId (TypingMode mode) { return (unsigned int)(mode + ID_OPTIONS_TYPING_INSERT); }
inline TypingMode   GetTypingMode(unsigned int id) { return (TypingMode)  (id   - ID_OPTIONS_TYPING_INSERT); }

#endif //EDITINGMODES_H
