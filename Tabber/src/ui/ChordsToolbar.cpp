#include "ChordsToolbar.h"
#include "../ui/MainWindow.h"

const int ChordsToolbar::CHORDS_TOOLBAR_HEIGHT = 55;
const int ChordsToolbar::CHORD_BUTTON_HEIGHT = 25;
const int ChordsToolbar::CHORD_BUTTON_WIDTH = 50;
const int ChordsToolbar::CHORD_BUTTON_PADDING = 2;


ChordsToolbar::ChordsToolbar(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;
	_panels = NULL;
	OBJECT_CREATED;
}


ChordsToolbar::~ChordsToolbar()
{
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
	
	//add tabs to the tab control
	TC_ITEM tabSettings;
	tabSettings.mask = TCIF_TEXT;
	for(int groupIndex=0; groupIndex < _panelCount; ++groupIndex)
	{
		//pszText member is not const (Why not ?) so I need a cast
		tabSettings.pszText = const_cast<char*>(chords->getChordGroupAt(groupIndex)->getName());
		if( TabCtrl_InsertItem(_hWindow, groupIndex, &tabSettings) == -1 )
		{
			throw new RuntimeException("ChordsToolbar::create", "Could not add a tab in toolbar");	
		}
	}
	
	//create child panels
	_panels = new HWND[_panelCount];
	for(int groupIndex=0; groupIndex < _panelCount; ++groupIndex)
	{
		ChordGroup* currentGroup = chords->getChordGroupAt(groupIndex);
		
		_panels[groupIndex] = CreateWindowEx(
			0,
			"STATIC", //create a specific class to propagage WM_COMMAND
			NULL,
			WS_CHILD,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			hParentWindow, NULL, GetModuleHandle(NULL), NULL );
			
		if(_panels[groupIndex] == NULL)
		{
			throw new RuntimeException("ChordsToolbar::create", "Could not add a tab in toolbar");			
		}
	
		//load buttons
		int chordCount = currentGroup->getChordCount();
		int buttonCommandId = IDC_FIRST_CHORD;
		for(int chordIndex=0; chordIndex < chordCount; ++chordIndex)
		{
			HWND hButton = CreateWindowEx(
				0,
				"BUTTON",
				currentGroup->getChordAt(chordIndex)->getName(),
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				chordIndex * (CHORD_BUTTON_WIDTH + CHORD_BUTTON_PADDING),
    			CHORD_BUTTON_PADDING,
       			CHORD_BUTTON_WIDTH,
				CHORD_BUTTON_HEIGHT,
				_panels[groupIndex],
    			(HMENU)buttonCommandId,
       			GetModuleHandle(NULL),
          		NULL );
          		
			if(hButton == NULL)
			{
				throw new RuntimeException("ChordsToolbar::createChordButton", "Could not create chord button");			
			}
			
			ApplyUsersDefaultFont(hButton);
		}
		hidePanel(groupIndex);
	}
	_activePanelIndex = 0;
	showPanel(_activePanelIndex);
}


void ChordsToolbar::resize(const RECT& newSize)
{
	assert(_hWindow != NULL && _panels[_activePanelIndex] != NULL);

	//sizing of the tab control and its child panels at once (using *DeferWindowPos)
	HDWP deferWindowPosStructure; 

    // Size the tab control to fit the client area. 
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

	    // Position and size the child windows to fit the 
	    // tab control's display area, and make sure the 
	    // static control is in front of the tab control. 
	    DeferWindowPos(
	    	deferWindowPosStructure, 
	        _panels[_activePanelIndex],
	        HWND_TOP,
			newSize.left, newSize.top, newSize.right - newSize.left, newSize.bottom - newSize.top,
			0 ); 
	}
    EndDeferWindowPos(deferWindowPosStructure); 
}


void ChordsToolbar::showPanel(int index)
{
	assert(index < _panelCount && _panels[index] != NULL);
	ShowWindow(_panels[index], SW_SHOW);
}


void ChordsToolbar::hidePanel(int index)
{
	assert(index < _panelCount && _panels[index] != NULL);
	ShowWindow(_panels[index], SW_HIDE);
}


void ChordsToolbar::updateOnTabChange()
{
	int newActivePanel = TabCtrl_GetCurSel(_hWindow);
	int oldActivePanel = _activePanelIndex;
	
	//active panel has been resized properly if needed, but inactive panels have not
	//so I resize them when they are displayed
/*
	RECT activePanelRect;
	GetWindowRect(_panels[oldActivePanel], &activePanelRect);
	SetWindowPos(_panels[newActivePanel], NULL, activePanelRect.left, activePanelRect.top, activePanelRect.right - activePanelRect.left, activePanelRect.bottom - activePanelRect.top, 0);
//*/

	//hide & seek :)
	hidePanel(oldActivePanel);	
	showPanel(newActivePanel);

	_activePanelIndex = newActivePanel;	
}


