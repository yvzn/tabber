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

#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

#include "../../util/utilities.h"
#include "../../core/ApplicationSettings.h"
#include "../../ui/dialogs/PromptDialog.h"

/* forward */ class MainWindow;


/**
 * Gateway between user interface and application settings
 */
class SettingsInterface
{

	public:

		SettingsInterface(MainWindow* );
		~SettingsInterface();

		void applySettings();
		void saveSettings();

		void onChooseFont();
		void onChangeTypingMode(TypingMode );
		void onToggleTypingMode( );
		void onChangeGuitarTuning(TuningIndex );
		void onToggleChordMode(ChordMode );
		void onChangeChordMode(ChordMode, bool);
		void onChooseStaffWidth();
		void onChooseChordDepth();

	protected:

		MainWindow* _mainWindow;

		CHOOSEFONT _fontDialogOptions;

};

#endif // SETTINGSINTERFACE_H

