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
		
		const RECT& getMainWindowRect           () const ;
        bool        getMainWindowMaximizedState () const ;

	protected:
	
		static const char APPLICATION_SETTINGS_FILE_NAME[];
		
		char _settingsFileFullName[MAX_PATH];
		RECT _mainWindowRect;
		bool _mainWindowMaximizedState;
};


#endif // APPLICATIONSETTINGS_H

