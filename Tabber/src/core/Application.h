#ifndef APPLICATION_H
#define APPLICATION_H

#include "../util/utilities.h"
#include "../core/ApplicationSettings.h"
#include "../data/ChordDefinitions.h"

/* forward */ class MainWindow;


class Application
{

	public:

		Application();
		virtual ~Application();

		void create  (HINSTANCE )       ;
		void show    (int )       const ;
		
		inline MainWindow*          getMainwindow       ( ) { return _mainWindow;   }
		inline ApplicationSettings* getSettings         ( ) { return _settings;     }
		inline ChordDefinitions*    getChordDefinitions ( ) { return _chords;       }

		bool translateAccelerator(MSG* );

	protected:
	
		MainWindow*          _mainWindow; 
		ApplicationSettings* _settings;
		ChordDefinitions*    _chords;
		HACCEL               _accelerators;

};


#endif // APPLICATION_H
