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

#include "PrinterInterface.h" 
#include "../../ui/MainWindow.h"


PrinterInterface::PrinterInterface(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;

	ZeroMemory(&_pageSetupOptions, sizeof(_pageSetupOptions));
	_pageSetupOptions.lStructSize = sizeof(_pageSetupOptions);
	_pageSetupOptions.Flags = PSD_DEFAULTMINMARGINS;

	ZeroMemory(&_printOptions, sizeof(_printOptions));
	_printOptions.lStructSize = sizeof(_printOptions);
	_printOptions.Flags = PD_ALLPAGES | PD_NOPAGENUMS | PD_NOSELECTION | PD_RETURNDC;
	_printOptions.nCopies = 1;

	_arePageSetupOptionsValid = false; // false until user chooses page setup

	OBJECT_CREATED;
}


PrinterInterface::~PrinterInterface()
{
	OBJECT_DELETED;
}


bool PrinterInterface::onChoosePageSetup()
{
	_pageSetupOptions.hwndOwner = _mainWindow->getWindowHandle();

	if(PageSetupDlg(&_pageSetupOptions))
	{
		_pageSetupOptions.Flags = PSD_MARGINS;
		_arePageSetupOptionsValid = true;
		return true;
	}
	else
	{
		return false;
	}
}


/**
 * Does the job :) Inspired by Petzold's "poppad.c"
 * Temporary version, could be improved -- for instance, do not split
 * staffs at page bottoms, remove these four for loops, simplify initialisations...
 * @see http://www.catch22.org.uk/tuts/printing.asp
 */
void PrinterInterface::onPrint()
{
	if(!_arePageSetupOptionsValid) // initialise if user did not
	{
		_pageSetupOptions.Flags |= PSD_RETURNDEFAULT; // return directly, do not show
		bool setupDone = onChoosePageSetup();
		_pageSetupOptions.Flags &= (~PSD_RETURNDEFAULT);
		if(!setupDone) return;
	}

	_printOptions.hwndOwner = _mainWindow->getWindowHandle();

	//retrieve device stuff
	_printOptions.hDevMode = _pageSetupOptions.hDevMode;
	_printOptions.hDevNames = _pageSetupOptions.hDevNames;

	if(PrintDlg(&_printOptions))
	{
		HDC& deviceContext = _printOptions.hDC;

		//compute margins
		RECT margins;
		CopyMemory(&margins, &_pageSetupOptions.rtMargin, sizeof(margins));
		convertToThousandthsOfInches(margins);

		margins.left   = MulDiv(margins.left,   GetDeviceCaps(deviceContext, LOGPIXELSX), 1000);
		margins.top    = MulDiv(margins.top,    GetDeviceCaps(deviceContext, LOGPIXELSY), 1000);
		margins.right  = MulDiv(margins.right,  GetDeviceCaps(deviceContext, LOGPIXELSX), 1000);
		margins.bottom = MulDiv(margins.bottom, GetDeviceCaps(deviceContext, LOGPIXELSY), 1000);

		//compute adjustments (difference between specified margins and device's own margins)
		RECT adjust;
		adjust.left   = margins.left   - GetDeviceCaps(deviceContext, PHYSICALOFFSETX);
		adjust.top    = margins.top    - GetDeviceCaps(deviceContext, PHYSICALOFFSETY);
		adjust.right  = margins.right  - (GetDeviceCaps(deviceContext, PHYSICALWIDTH)  - GetDeviceCaps(deviceContext, PHYSICALOFFSETX) - GetDeviceCaps(deviceContext, HORZRES));
		adjust.bottom = margins.bottom - (GetDeviceCaps(deviceContext, PHYSICALHEIGHT) - GetDeviceCaps(deviceContext, PHYSICALOFFSETY) - GetDeviceCaps(deviceContext, VERTRES));

		if(adjust.left   < 0) adjust.left   = 0;
		if(adjust.top    < 0) adjust.top    = 0;
		if(adjust.bottom < 0) adjust.bottom = 0;
		if(adjust.right  < 0) adjust.right  = 0;

		int iWidth  = GetDeviceCaps(deviceContext, HORZRES) - (adjust.left + adjust.right);
		int iHeight = GetDeviceCaps(deviceContext, VERTRES) - (adjust.top + adjust.bottom);

		//select font to compute text metircs
		HFONT printFont = _mainWindow->getEditArea()->getDisplayFont();
		HFONT hOldFont = (HFONT)SelectObject(deviceContext, printFont);

		//compute the number of lines per page
		TEXTMETRIC metrics;
		GetTextMetrics(deviceContext, &metrics);
		int charHeight = metrics.tmHeight;
		int charWidth = metrics.tmAveCharWidth;

		int iHeaderHeight   = 2;
		int iLinesPerPage   = (iHeight - iHeaderHeight) / charHeight;

		//pagination
		PrinterPager* pager = new PrinterPager(_mainWindow->getEditArea(), iLinesPerPage);
		int iTotalLines     = _mainWindow->getEditArea()->getToolkit()->getLineCount();
		int iTotalPages     = pager->getPageCount();
		int iLineNum        = 0;

		//document information
		DOCINFO docInfo;
		ZeroMemory(&docInfo, sizeof(docInfo));
		docInfo.cbSize = sizeof(docInfo);
		docInfo.lpszDocName = _mainWindow->getDocumentInterface()->getFileName();

		//show progress window and prepare printing cancelling
		PrintProgressDialog::show(
			_mainWindow->getWindowHandle(),
			_mainWindow->getDocumentInterface()->getFileName(),
			iTotalPages );
		SetAbortProc(deviceContext, PrintProgressDialog::AbortProc);

		//printing
		BOOL bSuccess = TRUE;
		BOOL bUserAbort = FALSE;
		char* outputBuffer = new char[1024];

		if(StartDoc(deviceContext, &docInfo) > 0)
		{
			for(int iColCopy = 0; iColCopy < ((_printOptions.Flags & PD_COLLATE) ? _printOptions.nCopies : 1); iColCopy++)
			{
				for(int iPage = 0; iPage < iTotalPages; iPage++)
				{
					PrintProgressDialog::setCurrentPage(iPage);

				    for(int iNonColCopy = 0; iNonColCopy < ((_printOptions.Flags & PD_COLLATE) ? 1 : _printOptions.nCopies); iNonColCopy++)
				    {
						if(StartPage(deviceContext) < 0)
						{
							bSuccess = FALSE;
							break;
						}

						//Make all printing be offset by the amount specified for the margins
						SetViewportOrgEx(deviceContext, adjust.left, adjust.top, NULL);

						//Fonts and mapping (must reselect on every page to be compatible with Win9x)
						SetMapMode(deviceContext, MM_TEXT);
						hOldFont = (HFONT)SelectObject(deviceContext, printFont);

						//print the current file line by line
						int iPageLines = pager->getPageLineCount(iPage);

						for(int iLine = 0; iLine < iPageLines; iLine++)
						{
							iLineNum = pager->getPageFirstLine(iPage) + iLine;
							if(iLineNum > iTotalLines) break;

							//get line (iLine) from the application, and store it into szBuffer
							unsigned int length = _mainWindow->getEditArea()->getToolkit()->getLine(iLineNum, outputBuffer);

							TextOut(
								deviceContext,
								0, charHeight * iLine,
								outputBuffer, length );

							bUserAbort = PrintProgressDialog::didUserCancelPrinting();
						}

						//print header / footer
						wsprintf(
							outputBuffer,
							"%s - Page %d/%d",
							_mainWindow->getDocumentInterface()->getFileName(),
							iPage+1, iTotalPages );
						TextOut(
							deviceContext,
							(iWidth - charWidth * lstrlen(outputBuffer))/2, charHeight * (iLinesPerPage + 1),
							outputBuffer, lstrlen(outputBuffer) );

						SelectObject(deviceContext, hOldFont);

						if(EndPage(deviceContext) < 0)
						{
							bSuccess = FALSE;
							break;
						}

						bUserAbort = PrintProgressDialog::didUserCancelPrinting();
						if(bUserAbort) break;
					}

					if(!bSuccess || bUserAbort) break;
				}

				if(!bSuccess || bUserAbort) break;
			}

		}
		else
		{
			bSuccess = FALSE;
		}

		if(bSuccess && !bUserAbort)
		{
			EndDoc(deviceContext);
		}
		else
		{
			AbortDoc(deviceContext);
			if(bUserAbort)
			{
				NotifyMessage::publicError(
					_mainWindow->getWindowHandle(),
					System::getLocaleString(IDERR_PRINT_CANCELLED) );
			}
			else
			{
				NotifyMessage::publicError(
					_mainWindow->getWindowHandle(),
					System::getLocaleString(IDERR_PRINT_FAILED) );
			}
		}

		PrintProgressDialog::hide();

		delete [] outputBuffer;

		delete pager;

		DeleteDC(deviceContext);
	}
}


/**
 * Converts rectangle to thousadths of inches, regarding to User's Locale settings.
 * If user's Locale unit system is metric, does the conversion, if not, leaves the rectangle as is
 */
void PrinterInterface::convertToThousandthsOfInches(RECT& rectangle)
{
	int width=GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, NULL, 0);
	char* localeInfo = new char[width];

	if(GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, localeInfo, width))
	{
		if(localeInfo[0] == '0') //metric system is used
		{
#define convert(x) ((int)(ceil(x * 100. / 254.)))

			rectangle.top    = convert(rectangle.top);
			rectangle.left   = convert(rectangle.left);
			rectangle.bottom = convert(rectangle.bottom);
			rectangle.right  = convert(rectangle.right);

#undef convert
		}
	}

	delete [] localeInfo;
}
