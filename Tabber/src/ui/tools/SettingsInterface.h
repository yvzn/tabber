#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

#include "../../util/utilities.h"
#include "../../core/ApplicationSettings.h"
#include "../../ui/PromptDialog.h"

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

