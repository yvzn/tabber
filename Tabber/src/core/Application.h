#ifndef APPLICATION_H
#define APPLICATION_H

#include "../core/utilities.h"
#include "../core/GlobalSettings.h"
#include "../data/ChordDefinitions.h"


class CApplication
{

	public:

		CApplication();
		virtual ~CApplication();

		void create  (HINSTANCE );
		void show    (int );
		
		CGlobalSettings*   getGlobalSettings   ();
		CChordDefinitions* getChordDefinitions (); 
		
	protected:
	
		CGlobalSettings*   m_lpSettings;
		CChordDefinitions* m_lpChords;
};


#endif // APPLICATION_H
