/*
 * Tabber - Guitar Tablature Editor, freeware and open-source
 * Copyright (C) 2004  Yvan Razafindramanana
 *
 * Additionnal Contributors:
 * <place your name here :)>
 *
 * This file is part of Tabber.
 * 
 * Tabber is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tabber is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tabber.  If not, see <https://www.gnu.org/licenses/>.
 */

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
		bool interceptsMessage  (MSG* );

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

