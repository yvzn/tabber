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

#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "../util/utilities.h"
#include "../ui/tools/EditionModes.h"


class ApplicationSettings
{

	public:

		ApplicationSettings();
		virtual ~ApplicationSettings ();

		void load ()       ;
		void save () const ;

		inline const RECT&    getMainWindowRect           () const { return _mainWindowRect;           }
		inline bool           isMainWindowMaximized       () const { return _isMainWindowMaximized;    }
		inline const LOGFONT& getEditAreaFont             () const { return _editAreaFont;             }
		inline TypingMode     getTypingMode               () const { return _typingMode;               }
		inline TuningIndex    getSelectedTuningIndex      () const { return _tuningIndex;              } //< @return 0 if no tuning is selected
		inline bool           isChordModeEnabled (ChordMode mode) const { return _isChordModeEnabled[mode]; }
		inline unsigned int   getStaffWidth               () const { return _staffWidth;               }
		inline unsigned int   getChordDepth               () const { return _chordDepth;               }

		inline void setMainWindowRect           (const RECT&    val) { CopyMemory(&_mainWindowRect, &val, sizeof(RECT));         }
		inline void setMainWindowMaximized      (bool           val) { _isMainWindowMaximized = val;    }
		inline void setEditAreaFont             (const LOGFONT& val) { CopyMemory(&_editAreaFont, &val, sizeof(_editAreaFont));  }
		inline void setTypingMode               (TypingMode     val) { _typingMode = val;               }
		inline void setSelectedTuningIndex      (TuningIndex    val) { _tuningIndex = val;              }
		inline void setChordModeEnabled   (ChordMode mode, bool val) { _isChordModeEnabled[mode] = val; }
		inline void setStaffWidth               (unsigned int   val) { _staffWidth = val;               }
		inline void setChordDepth               (unsigned int   val) { _chordDepth = val;               }

	protected:
	
		RECT        _mainWindowRect;
		bool        _isMainWindowMaximized;
		LOGFONT     _editAreaFont;
		
		TypingMode   _typingMode;
		TuningIndex  _tuningIndex;
		bool         _isChordModeEnabled[CHORD_MODE_COUNT];
		unsigned int _staffWidth;
		unsigned int _chordDepth;

	private:

		static const char APPLICATION_SETTINGS_FILE_NAME[];
		             char _settingsFileFullName[MAX_PATH];

};


#endif // APPLICATIONSETTINGS_H

