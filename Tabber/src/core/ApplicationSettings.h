#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "../util/utilities.h"
#include "../ui/EditingModes.h"


class ApplicationSettings
{

	public:

		ApplicationSettings();
		virtual ~ApplicationSettings ();

		void load ()       ;
		void save () const ;

		void setMainWindowRect           (const RECT&    );
		void setMainWindowMaximizedState (bool           );
		void setEditAreaFont             (const LOGFONT& );
		void setEditAreaTypingMode       (TypingMode     );

		inline const RECT&    getMainWindowRect           () const { return _mainWindowRect;           }
        inline bool           getMainWindowMaximizedState () const { return _mainWindowMaximizedState; }
		inline const LOGFONT& getEditAreaFont             () const { return _editAreaFont;             }
		inline TypingMode     getEditAreaTypingMode       () const { return _editAreaTypingMode;       }

	protected:
	
		static const char APPLICATION_SETTINGS_FILE_NAME[];
		
		char _settingsFileFullName[MAX_PATH];
		
		RECT       _mainWindowRect;
		bool       _mainWindowMaximizedState;
		LOGFONT    _editAreaFont;
		TypingMode _editAreaTypingMode;

};


#endif // APPLICATIONSETTINGS_H

