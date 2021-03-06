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

#include <windows.h>

#define IDC_STATIC -1

#define IDR_MAIN_MENU 1000
#define IDR_ACCELERATORS 1001

#define IDC_MAIN_TOOLBAR   2000
#define IDC_MAIN_STATUSBAR 2001
#define IDC_CHORDS_TOOLBAR 2002
#define IDC_EDIT_AREA      2003

#define IDI_ICON_LARGE 3000
#define IDI_ICON_SMALL 3001

#define ID_FILE_NEW        5000
#define ID_FILE_OPEN       5001
#define ID_FILE_SAVE       5002
#define ID_FILE_SAVEAS     5003
#define ID_FILE_PAGESETUP  5024
#define ID_FILE_PRINT      5025
#define ID_APP_EXIT        5004

#define ID_EDIT_UNDO        5008
#define ID_EDIT_CUT         5005
#define ID_EDIT_COPY        5006
#define ID_EDIT_PASTE       5007
#define ID_EDIT_DELETE      5016
#define ID_EDIT_SELECTALL   5015
#define ID_EDIT_FIND        5021
#define ID_EDIT_FINDNEXT    5022
#define ID_EDIT_REPLACE     5023

#define ID_INSERT_STAFF  5018
#define ID_INSERT_BAR    5019
#define ID_INSERT_TUNING 5020

#define ID_OPTIONS_FONT              5200
#define ID_OPTIONS_TYPING_TOGGLE     5201
#define ID_OPTIONS_TYPING_INSERT     5202
#define ID_OPTIONS_TYPING_OVERWRITE  5203
#define ID_OPTIONS_TYPING_SPECIAL    5204
#define ID_OPTIONS_CHORD_EXTRA_SPACE 5205
#define ID_OPTIONS_CHORD_NAME        5206
#define ID_OPTIONS_CHORD_ARPEGGIO    5207
#define ID_OPTIONS_STAFF_WIDTH       5208
#define ID_OPTIONS_STAFF_HEIGHT      5209
#define ID_OPTIONS_EDIT_CHORDS       5210
#define ID_OPTIONS_EDIT_TUNINGS      5211
#define ID_OPTIONS_TUNINGS_NONE      5901    // any higher ID is considered as a Tuning ID

#define ID_HELP_ABOUT  5010

#define IDC_FIRST_CHORD 32768  // any higher control ID is considered as a Chord ID (0x7F00 & group) | (0x00FF & chord)

#define IDD_ABOUT_DIALOG 4000
#define IDC_VERSION_NUMBER 2004
#define IDC_LICENCE 2010
#define IDC_WEBSITE 2011

#define IDD_DEBUG_DIALOG 4001
#define IDC_DEBUG_OUTPUT 2005

#define IDD_PROMPT_DIALOG 4002
#define IDC_PROMPT_STRING 2006
#define IDC_PROMPT_VALUE 2007

#define IDD_PRINT_DIALOG 4003
#define IDC_PRINT_DOCUMENT 2008
#define IDC_PRINT_STATUS 2009

#define IDS_UNTITLED                16000
#define IDS_SAVE_CHANGES            16001
#define IDS_CONFIRMATION            16002
#define IDS_STAFF_HEIGHT_PROMPT     16003
#define IDS_STAFF_WIDTH_PROMPT      16004
#define IDS_STAFF_TOOLBAR_CAPTION   16005
#define IDS_NO_MORE_OCCURENCES      16006
#define IDS_X_OCCURENCES_REPLACED   16007
#define IDS_TYPE_OVERWRITE          16008
#define IDS_TYPE_INSERT             16009
#define IDS_TYPE_SPECIAL            16010
#define IDS_INSERT_STAFF            16011
#define IDS_INSERT_BAR              16012
#define IDS_INSERT_TUNING           16013
#define IDS_CHANGES_ON_RESTART      16014
#define IDS_LICENCE                 16015
#define IDS_WEBSITE                 16016

#define IDERR_SAVE_CHORDS            24000
#define IDERR_OPEN_CHORDS_FILE       24001
#define IDERR_SAVE_TUNINGS           24002
#define IDERR_OPEN_TUNINGS_FILE      24003
#define IDERR_REGISTER_WINDOW_CLASS  24004
#define IDERR_CREATE_WINDOW          24005
#define IDERR_SAVE_DOCUMENT          24006
#define IDERR_OPEN_DOCUMENT          24007
#define IDERR_PRINT_CANCELLED        24008
#define IDERR_PRINT_FAILED           24009
#define IDERR_CREATE_TABS            24010

