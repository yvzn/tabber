#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

#include "../util/utilities.h"
#include "../core/ApplicationSettings.h"

/* forward */ class MainWindow;


/**
 * Gateway between user interface and application settings
 */
class SettingsInterface
{

	public:

		SettingsInterface(MainWindow* );
		~SettingsInterface();

		void onChooseFont();
		
	protected:
	    
	    MainWindow* _mainWindow;
	    
	    CHOOSEFONT _fontDialogOptions;

};

#endif // SETTINGSINTERFACE_H

