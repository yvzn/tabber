#ifndef APPLICATION_H
#define APPLICATION_H

#include "../util/utilities.h"
#include "../core/ApplicationSettings.h"
#include "../data/ChordDefinitions.h"


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
	
		ApplicationSettings* _settings;
		ChordDefinitions*    _chords;

};


#endif // APPLICATION_H
