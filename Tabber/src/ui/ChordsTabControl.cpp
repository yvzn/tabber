#include "ChordsTabControl.h"
#include "../ui/MainWindow.h"

const int ChordsTabControl::CHORDS_TOOLBAR_HEIGHT = 55;


ChordsTabControl::ChordsTabControl(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;
	_panels = NULL;
	_panelCount = 0;
	OBJECT_CREATED;
}


ChordsTabControl::~ChordsTabControl()
{
	for(int panelIndex=1; panelIndex < _panelCount; ++panelIndex)
	{
		if(_panels[panelIndex] != NULL) delete _panels[panelIndex];
	}

	if(_panels != NULL) delete [] _panels;
	OBJECT_DELETED;
}


void ChordsTabControl::create(HWND hParentWindow)
{
	_panelCount = _mainWindow->getApplication()->getChordDefinitions()->getChordGroupCount() + 1; //+1 for the StaffToolbar
	
	//tab control itself
	_hWindow = CreateWindowEx (
		0,
		WC_TABCONTROL,
		NULL,
		WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hParentWindow, (HMENU)IDC_CHORDS_TOOLBAR, GetModuleHandle(NULL), NULL );

	if(_hWindow == NULL)
	{
		throw new RuntimeException("ChordsTabControl::create", System::getLocaleString(IDERR_CREATE_WINDOW));
	}

	System::applyGUIDefaultFont(_hWindow);
	
	createTabs(hParentWindow);
	createChildPanels(hParentWindow);

	_activePanelIndex = 0;
	_panels[_activePanelIndex]->show();
}


void ChordsTabControl::createTabs(HWND hParentWindow)
{
	assert(_hWindow != NULL && _panelCount > 0);
	ChordDefinitions* chords = _mainWindow->getApplication()->getChordDefinitions();

	TC_ITEM tabSettings;
	tabSettings.mask = TCIF_TEXT;

	//the first tab is the staff toolbar
	tabSettings.pszText = const_cast<char*>(System::getLocaleString(IDS_STAFF_TOOLBAR_CAPTION));
	if( TabCtrl_InsertItem(_hWindow, 0, &tabSettings) == -1 )
	{
		throw new RuntimeException("ChordsTabControl::createTabs", System::getLocaleString(IDERR_CREATE_TABS));
	}

	//the following ones are the chords toolbars
	for(int groupIndex=0; groupIndex < chords->getChordGroupCount(); ++groupIndex)
	{
		//pszText member is not const (Why not ?) so I need a cast
		tabSettings.pszText = const_cast<char*>(chords->getChordGroupAt(groupIndex)->getName());
		if( TabCtrl_InsertItem(_hWindow, groupIndex+1, &tabSettings) == -1 )
		{
			throw new RuntimeException("ChordsTabControl::createTabs", System::getLocaleString(IDERR_CREATE_TABS));
		}
	}
}


void ChordsTabControl::createChildPanels(HWND hParentWindow)
{
	assert(_hWindow != NULL && _panelCount > 0);
	ChordDefinitions* chords = _mainWindow->getApplication()->getChordDefinitions();

	_panels = new ChordsTabControlPanel*[_panelCount];
	ZeroMemory(_panels, sizeof(_panels));

	//the first panel is the staff toolbar
	_panels[0] = _mainWindow->getStaffToolbar();
	_panels[0]->hide();

	//the following ones are the chords toolbars
	for(int groupIndex=0; groupIndex < chords->getChordGroupCount(); ++groupIndex)
	{
		ChordGroup* currentGroup = chords->getChordGroupAt(groupIndex);
		ChordsToolbar* toolbar = new ChordsToolbar(this);

		try
		{
			toolbar->create(hParentWindow);

			//load buttons
			int chordCount = currentGroup->getChordCount();
			for(int chordIndex=0; chordIndex < chordCount; ++chordIndex)
			{
				ChordIndex index = { groupIndex, chordIndex };

				toolbar->addButton(
					currentGroup->getChordAt(chordIndex)->getName(),
					GetCommandId(index) );
			}

			_panels[groupIndex+1] = toolbar;
		}
		catch(RuntimeException* ex)
		{
			throw new RuntimeException("ChordsTabControl::create", ex);
		}

		_panels[groupIndex]->hide();
	}
}


void ChordsTabControl::resize(const RECT& newSize)
{
	assert(_hWindow != NULL && _panels != NULL);

	//sizing of the tab control and its child panels at once (using *DeferWindowPos)
	HDWP deferWindowPosStructure; 
    deferWindowPosStructure = BeginDeferWindowPos(2); 
    {
    	DeferWindowPos(
    		deferWindowPosStructure,
     		_hWindow,
      		NULL,
			newSize.left, newSize.top, newSize.right - newSize.left, newSize.bottom - newSize.top, 
	        SWP_NOZORDER );

		//re-adjust size for child panels
		TabCtrl_AdjustRect(_hWindow, FALSE, &newSize);

	    DeferWindowPos(
	    	deferWindowPosStructure, 
	        _panels[_activePanelIndex]->getWindowHandle(),
	        HWND_TOP,
			newSize.left, newSize.top, newSize.right - newSize.left, newSize.bottom - newSize.top,
			0 ); 
	}
    EndDeferWindowPos(deferWindowPosStructure); 
}


void ChordsTabControl::updateOnTabChange()
{
	int newActivePanel = TabCtrl_GetCurSel(_hWindow);
	int oldActivePanel = _activePanelIndex;
	
	//hide & seek :)
	_panels[oldActivePanel]->hide();
	_panels[newActivePanel]->show();

	_activePanelIndex = newActivePanel;	
}


