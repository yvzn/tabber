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


void SettingsInterface::onChooseFont()
{
    ApplicationSettings* settings = _mainWindow->getApplication()->getSettings();
    
    //initialise default values (using app. settings)
    LOGFONT font;
	CopyMemory(&font, &settings->getEditAreaFont(), sizeof(font));

    //as I store size units in points in the Settings, I need a conversion from point size to logical units here
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

