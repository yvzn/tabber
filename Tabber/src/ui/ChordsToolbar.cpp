#include "ChordsToolbar.h"
#include "../ui/MainWindow.h"

const int ChordsToolbar::CHORDS_TOOLBAR_HEIGHT = 55;


ChordsToolbar::ChordsToolbar(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;
	_panels = NULL;
	_panelCount = 0;
	OBJECT_CREATED;
}


ChordsToolbar::~ChordsToolbar()
{
	for(int panelIndex=0; panelIndex < _panelCount; ++panelIndex)
	{
		if(_panels[panelIndex] != NULL) delete _panels[panelIndex];
	}

	if(_panels != NULL) delete [] _panels;
	OBJECT_DELETED;
}


void ChordsToolbar::create(HWND hParentWindow)
{
	assert(_mainWindow != NULL);

	ChordDefinitions* chords = _mainWindow->getApplication()->getChordDefinitions();
	_panelCount = chords->getChordGroupCount();
	
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
		throw new RuntimeException("ChordsToolbar::create", "Could not create chords toolbar");	
	}

	ApplyUsersDefaultFont(_hWindow);
	int groupIndex;
	
	//add tabs to the tab control
	TC_ITEM tabSettings;
	tabSettings.mask = TCIF_TEXT;
	for(groupIndex=0; groupIndex < _panelCount; ++groupIndex)
	{
		//pszText member is not const (Why not ?) so I need a cast
		tabSettings.pszText = const_cast<char*>(chords->getChordGroupAt(groupIndex)->getName());
		if( TabCtrl_InsertItem(_hWindow, groupIndex, &tabSettings) == -1 )
		{
			throw new RuntimeException("ChordsToolbar::create", "Could not add a tab in toolbar");	
		}
	}
	
	//create child panels
	_panels = new ChordButtonsPanel*[_panelCount];
	ZeroMemory(_panels, sizeof(_panels));

	for(groupIndex=0; groupIndex < _panelCount; ++groupIndex)
	{
		ChordGroup* currentGroup = chords->getChordGroupAt(groupIndex);
		_panels[groupIndex] = new ChordButtonsPanel(this);

		try
		{	
			_panels[groupIndex]->create(hParentWindow);
			
			//load buttons
			int chordCount = currentGroup->getChordCount();
			for(int chordIndex=0; chordIndex < chordCount; ++chordIndex)
			{
				ChordIndex index = { groupIndex, chordIndex };

				_panels[groupIndex]->addButton(
					currentGroup->getChordAt(chordIndex)->getName(),
					GetCommandId(index) );
			}
		}
		catch(RuntimeException* ex)
		{
			throw new RuntimeException("ChordsToolbar::create", ex);
		}

		_panels[groupIndex]->hide();
	}
	_activePanelIndex = 0;
	_panels[_activePanelIndex]->show();
}


void ChordsToolbar::resize(const RECT& newSize)
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


void ChordsToolbar::updateOnTabChange()
{
	int newActivePanel = TabCtrl_GetCurSel(_hWindow);
	int oldActivePanel = _activePanelIndex;
	
	//hide & seek :)
	_panels[oldActivePanel]->hide();
	_panels[newActivePanel]->show();

	_activePanelIndex = newActivePanel;	
}


