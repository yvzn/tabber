#include "SettingsInterface.h"
#include "../../ui/MainWindow.h"


SettingsInterface::SettingsInterface(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;

	ZeroMemory(&_fontDialogOptions, sizeof(_fontDialogOptions));	
    _fontDialogOptions.lStructSize = sizeof(_fontDialogOptions); 
    _fontDialogOptions.Flags = CF_SCREENFONTS | CF_FIXEDPITCHONLY | CF_FORCEFONTEXIST | CF_INITTOLOGFONTSTRUCT; 
	
	OBJECT_CREATED;
}


SettingsInterface::~SettingsInterface()
{
	OBJECT_DELETED;
}


/**
 * Once application's settings (such as window size, font, etc.) have been loaded, applies/displays them to/in the main window
 */
void SettingsInterface::applySettings()
{
    ApplicationSettings* settings = _mainWindow->getApplication()->getSettings();
    HWND hMainWindow = _mainWindow->getWindowHandle();

	//window's size
	const RECT windowRect = settings->getMainWindowRect();
	if(windowRect.top > 0) // rect is valid
	{
		int x, y, width, height;
		x = windowRect.left; width = windowRect.right - windowRect.left;
		y = windowRect.top; height = windowRect.bottom - windowRect.top;
		
		SetWindowPos(hMainWindow, HWND_TOP, x, y, width, height, 0);
	}
	
	//window's maximized state
	if(settings->isMainWindowMaximized())
 	{
       ShowWindow(hMainWindow, SW_MAXIMIZE);
	}

	//edit area's font
	LOGFONT font;
 	CopyMemory(&font, &settings->getEditAreaFont(), sizeof(font));
    //as I store size units in points in the Settings, I need a conversion from point size to logical units here
	font.lfHeight = -MulDiv(font.lfHeight, GetDeviceCaps(GetDC(_mainWindow->getWindowHandle()), LOGPIXELSY), 72);
	_mainWindow->getEditArea()->setFont(font);
	
	//guitar tunings (requires menu item creation)
	HMENU mainMenu = _mainWindow->getMenu();
	TuningDefinitions* tunings = _mainWindow->getApplication()->getTuningDefinitions();
	unsigned int tuningCount = tunings->getTuningCount();
 	for(TuningIndex index=0; index<tuningCount; ++index)
 	{
		InsertMenu(
  			mainMenu,
  			ID_OPTIONS_TUNINGS_NONE,
  			MF_BYCOMMAND,
  			GetCommandId(index+1),
  			tunings->getTuningAt(index)->getName() );
	}
	InsertMenu(mainMenu, ID_OPTIONS_TUNINGS_NONE, MF_BYCOMMAND | MF_SEPARATOR, 0, NULL);
	onChangeGuitarTuning(settings->getSelectedTuningIndex());

	//typing mode
	onChangeTypingMode(settings->getTypingMode());

	//chord modes
	onChangeChordMode(ADD_EXTRA_SPACE, settings->isChordModeEnabled(ADD_EXTRA_SPACE));
	onChangeChordMode(ADD_NAME,        settings->isChordModeEnabled(ADD_NAME)       );
}



void SettingsInterface::saveSettings()
{
	ApplicationSettings* settings = _mainWindow->getApplication()->getSettings();
    HWND hMainWindow = _mainWindow->getWindowHandle();

	RECT windowRect;
	GetWindowRect(hMainWindow, &windowRect);
	settings->setMainWindowRect(windowRect);
	
	settings->setMainWindowMaximized(IsZoomed(hMainWindow) > 0);
	
	//edit area's font is set separately
	
	settings->save();
}


void SettingsInterface::onChooseFont()
{
    ApplicationSettings* settings = _mainWindow->getApplication()->getSettings();
    
    LOGFONT font;
	CopyMemory(&font, &settings->getEditAreaFont(), sizeof(font));

	font.lfHeight = -MulDiv(font.lfHeight, GetDeviceCaps(GetDC(_mainWindow->getWindowHandle()), LOGPIXELSY), 72);
    
    _fontDialogOptions.hwndOwner = _mainWindow->getWindowHandle(); 
	_fontDialogOptions.lpLogFont = &font;
	
	if(ChooseFont(&_fontDialogOptions))
	{
	    _mainWindow->getEditArea()->setFont(font);
	    
	    //backward conversion (easier thanks to iPointSize member)
	    font.lfHeight = _fontDialogOptions.iPointSize / 10;
	    settings->setEditAreaFont(font);
	}
}


void SettingsInterface::onChangeTypingMode(TypingMode newMode)
{
    _mainWindow->getApplication()->getSettings()->setTypingMode(newMode);
    
	//update menus
	CheckMenuRadioItem(
 		_mainWindow->getMenu(),
 		ID_OPTIONS_TYPING_INSERT,
 		ID_OPTIONS_TYPING_SPECIAL,
 		GetCommandId(newMode),
 		MF_BYCOMMAND );

	//update status bar
	_mainWindow->getStatusBar()->updateTypingMode(newMode);
}


/**
 * Circles inside typing modes
 */
void SettingsInterface::onToggleTypingMode()
{
    TypingMode mode = _mainWindow->getApplication()->getSettings()->getTypingMode();
    mode = (TypingMode)((mode + 1) % TYPING_MODE_COUNT);
    onChangeTypingMode(mode);
}


void SettingsInterface::onChangeGuitarTuning(TuningIndex newTuning)
{
    _mainWindow->getApplication()->getSettings()->setSelectedTuningIndex(newTuning);

	//update menus
	CheckMenuRadioItem(
 		_mainWindow->getMenu(),
 		ID_OPTIONS_TUNINGS_NONE,
 		ID_OPTIONS_TUNINGS_NONE + _mainWindow->getApplication()->getTuningDefinitions()->getTuningCount(),
 		GetCommandId(newTuning),
 		MF_BYCOMMAND );
}


void SettingsInterface::onToggleChordMode(ChordMode mode)
{
	onChangeChordMode(mode, ! _mainWindow->getApplication()->getSettings()->isChordModeEnabled(mode));
}


void SettingsInterface::onChangeChordMode(ChordMode mode, bool isModeEnabled)
{
	_mainWindow->getApplication()->getSettings()->setChordModeEnabled(mode, isModeEnabled);
	
	//update menus
	unsigned int checkFlags = MF_BYCOMMAND | ( isModeEnabled ? MF_CHECKED : MF_UNCHECKED );
	CheckMenuItem(
 		_mainWindow->getMenu(),
 		GetCommandId(mode),
		checkFlags );
}


void SettingsInterface::onChooseStaffWidth()
{
    ApplicationSettings* settings = _mainWindow->getApplication()->getSettings();
    
	int action = PromptDialog::prompt(
 		_mainWindow->getWindowHandle(),
		System::getLocaleString(IDS_STAFF_WIDTH_PROMPT),
 		settings->getStaffWidth() );
 		
	if(action == IDOK)
	{
		settings->setStaffWidth(PromptDialog::getIntegerValue());
	}
}


void SettingsInterface::onChooseChordDepth()
{
    ApplicationSettings* settings = _mainWindow->getApplication()->getSettings();

	int action = PromptDialog::prompt(
 		_mainWindow->getWindowHandle(),
		System::getLocaleString(IDS_STAFF_HEIGHT_PROMPT),
  		settings->getChordDepth() );

	if(action == IDOK)
	{
		settings->setChordDepth(PromptDialog::getIntegerValue());
		_mainWindow->getEditArea()->onSelectionChange();
	}
}

