/*
 * Tabber - Guitar Tablature Editor, freeware and open-source
 * Copyright (C) 2004  Yvan Razafindramanana
 *
 * Additionnal Contributors:
 * <place your name here :)>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef PRINTPROGRESSDIALOG_H
#define PRINTPROGRESSDIALOG_H

#include "../../util/utilities.h"


/**
 * Shows a dialog where current printing job progress is shown with the possibility to cancel printing
 */
class PrintProgressDialog
{

	public:

		static void show(HWND , const char* , int );
		static void hide();

		static bool didUserCancelPrinting();
		static void setCurrentPage(int page);

		static BOOL CALLBACK AbortProc(HDC , int );

	protected:

		static BOOL CALLBACK handleMessage(HWND, UINT, WPARAM, LPARAM);

		static void onClose();

	protected:

		static HWND        _hWindow;

		static const char* _documentName;
		static char        _status[128];
		static int         _totalPages;
		static bool        _continuePrinting;

};

#endif // PRINTPROGRESSDIALOG_H

