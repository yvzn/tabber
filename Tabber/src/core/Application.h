#ifndef APPLICATION_H
#define APPLICATION_H

#include "../util/utilities.h"
#include "../core/ApplicationSettings.h"
#include "../data/ChordDefinitions.h"
#include "../data/TuningDefinitions.h"

/* forward */ class MainWindow;


class Application
{

	public:

		Application();
		virtual ~Application();

		void create  (HINSTANCE ) ;
		void show    (int )       const ;

		void processCommandLine (const char* );
		bool interceptMessage   (MSG* );

		inline ApplicationSettings* getSettings          ( ) { return _settings;     }
		inline ChordDefinitions*    getChordDefinitions  ( ) { return _chords;       }
		inline TuningDefinitions*   getTuningDefinitions ( ) { return _tunings;      }

	protected:
	
		MainWindow*          _mainWindow; 
		ApplicationSettings* _settings;
		ChordDefinitions*    _chords;
		TuningDefinitions*   _tunings;
		HACCEL               _accelerators;

};


#endif // APPLICATION_H
