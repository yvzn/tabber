#include "SettingsInterface.h"
#include "../ui/MainWindow.h"


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
 * Apply application's settings (such as window size, font, etc.) to the main window
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
	if(settings->getMainWindowMaximizedState())
 	{
       ShowWindow(hMainWindow, SW_MAXIMIZE);
	}

	//edit area's font
	LOGFONT font;
 	CopyMemory(&font, &settings->getEditAreaFont(), sizeof(font));
    //as I store size units in points in the Settings, I need a conversion from point size to logical units here
	font.lfHeight = -MulDiv(font.lfHeight, GetDeviceCaps(GetDC(_mainWindow->getWindowHandle()), LOGPIXELSY), 72);
	_mainWindow->getEditArea()->setFont(font);
	
	//edit area's typing mode
	onChangeTypingMode(settings->getEditAreaTypingMode());
}



void SettingsInterface::saveSettings()
{
	ApplicationSettings* settings = _mainWindow->getApplication()->getSettings();
    HWND hMainWindow = _mainWindow->getWindowHandle();

	RECT windowRect;
	GetWindowRect(hMainWindow, &windowRect);
	settings->setMainWindowRect(windowRect);
	
	settings->setMainWindowMaximizedState(IsZoomed(hMainWindow) > 0);
	
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


void SettingsInterface::onChangeTypingMode(TypingMode newValue)
{
    _mainWindow->getApplication()->getSettings()->setEditAreaTypingMode(newValue);
	_mainWindow->updateTypingMode(newValue);
}


/**
 * Circles inside typing modes
 */
void SettingsInterface::onToggleTypingMode()
{
    TypingMode currentMode = _mainWindow->getApplication()->getSettings()->getEditAreaTypingMode();
    currentMode = (TypingMode)((currentMode + 1) % TYPING_MODE_COUNT);
    onChangeTypingMode(currentMode);
}

