#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include "../core/utilities.h"


class CGlobalSettings
{

	public:

		CGlobalSettings  ();
		~CGlobalSettings ();

		void load ();
		void save ();

		int getMainWindowWidth  ();
		int getMainWindowHeight ();
		int getMainWindowX      ();
		int getMainWindowY      ();

		void setMainWindowWidth  (int );
		void setMainWindowHeight (int );
		void setMainWindowX      (int );
		void setMainWindowY      (int );

	protected:
	
		static const char SZ_GLOBAL_SETTINGS_FILE_NAME[];
		
		char m_szCompleteFileName[MAX_PATH];
		
		int m_cxMainWindowSize;
		int m_cyMainWindowSize;
		int m_xMainWindowPosition;
		int m_yMainWindowPosition;

};


#endif // GLOBALSETTINGS_H

