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
		
		ApplicationSettings* getSettings         ( ) ;
		ChordDefinitions*    getChordDefinitions ( ) ; 

	protected:
	
		MainWindow*          _mainWindow; 
		ApplicationSettings* _settings;
		ChordDefinitions*    _chords;

};


#endif // APPLICATION_H
