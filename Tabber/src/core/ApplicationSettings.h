#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include "../util/utilities.h"


class ApplicationSettings
{

	public:

		ApplicationSettings();
		virtual ~ApplicationSettings ();

		void load ()       ;
		void save () const ;

		void setMainWindowRect           (const RECT& );
		void setMainWindowMaximizedState (bool );
		void setEditAreaFont             (const LOGFONT& );

		const RECT&    getMainWindowRect           () const ;
        bool           getMainWindowMaximizedState () const ;
		const LOGFONT& getEditAreaFont()              const ;

	protected:
	
		static const char APPLICATION_SETTINGS_FILE_NAME[];
		
		char _settingsFileFullName[MAX_PATH];
		
		RECT    _mainWindowRect;
		bool    _mainWindowMaximizedState;
		LOGFONT _editAreaFont;

};


#endif // APPLICATIONSETTINGS_H

