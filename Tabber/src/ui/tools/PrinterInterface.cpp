#include "PrinterInterface.h" 
#include "../../ui/MainWindow.h"


PrinterInterface::PrinterInterface(MainWindow* parentWindow)
{
	_mainWindow = parentWindow;

	ZeroMemory(&_pageSetupOptions, sizeof(_pageSetupOptions));
	_pageSetupOptions.lStructSize = sizeof(_pageSetupOptions);
	_pageSetupOptions.rtMargin.top = 200;
	_pageSetupOptions.rtMargin.left = 1000;
	_pageSetupOptions.rtMargin.bottom = 1000;
	_pageSetupOptions.rtMargin.right = 1000;
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
 * Does the job :)
 * @see http://www.catch22.org.uk/tuts/printing.asp
 */
void PrinterInterface::onPrint()
{
	if(!_arePageSetupOptionsValid) // initialise if user did not
	{
		_pageSetupOptions.Flags |= PSD_RETURNDEFAULT; // return directly, do not show
  		if(!onChoosePageSetup())
		{
			throw new RuntimeException("PrinterInterface::onPrint", "Could not initialise Default Page Setup");
		}
		_pageSetupOptions.Flags &= (~PSD_RETURNDEFAULT);
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

		int iPhysOffsetX = GetDeviceCaps(deviceContext, PHYSICALOFFSETX);
		int iPhysOffsetY = GetDeviceCaps(deviceContext, PHYSICALOFFSETY);
		int iPhysWidth   = GetDeviceCaps(deviceContext, PHYSICALWIDTH);
		int iPhysHeight  = GetDeviceCaps(deviceContext, PHYSICALHEIGHT);

		int iLeftAdjust   = margins.left  - iPhysOffsetX;
		int iTopAdjust    = margins.top   - iPhysOffsetY;
		int iRightAdjust  = margins.right - (iPhysWidth  - iPhysOffsetX - GetDeviceCaps(deviceContext, HORZRES));
		int iBottomAdjust = margins.right - (iPhysHeight - iPhysOffsetY - GetDeviceCaps(deviceContext, VERTRES));

		int iWidth  = GetDeviceCaps(deviceContext, HORZRES) - (iLeftAdjust + iRightAdjust);
		int iHeight = GetDeviceCaps(deviceContext, VERTRES) - (iTopAdjust + iBottomAdjust);

		//fonts and mappings (must re-set on every page (to be compatible with Win9x)
		HFONT printFont = _mainWindow->getEditArea()->getDisplayFont();
		SetMapMode(deviceContext, MM_TEXT);
		HFONT hOldFont = (HFONT)SelectObject(deviceContext, printFont);

		//compute the number of lines per page
		TEXTMETRIC metrics;
		GetTextMetrics(deviceContext, &metrics);
		int charHeight = metrics.tmHeight;

		int iHeaderHeight   = 1;
		int iTotalLines     = _mainWindow->getEditArea()->getToolkit()->getLineCount();
		int iLinesPerPage   = (iHeight - iHeaderHeight) / charHeight;
		int iTotalPages     = (iTotalLines + iLinesPerPage - 1) / iLinesPerPage;
		int iLineNum        = 0;

		//document information
		DOCINFO docInfo;
		ZeroMemory(&docInfo, sizeof(docInfo));
		docInfo.cbSize = sizeof(docInfo);
		docInfo.lpszDocName = _mainWindow->getDocumentInterface()->getFileName();

		//printing
		BOOL bSuccess = TRUE;
		BOOL bUserAbort = FALSE;

        if(StartDoc(deviceContext, &docInfo) > 0)
        {
			PrintProgressDialog::show(
				_mainWindow->getWindowHandle(),
				_mainWindow->getDocumentInterface()->getFileName(),
				iTotalPages );

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
                        SetViewportOrgEx(deviceContext, iLeftAdjust, iTopAdjust, NULL);
 						SetViewportExtEx(deviceContext, iWidth, iHeight, NULL); //buggy !

                        hOldFont = (HFONT)SelectObject(deviceContext, printFont);

                        //print the current file line by line
                        for(int iLine = 0; iLine < iLinesPerPage; iLine++)
                        {
                            char* szBuffer = NULL;
                            iLineNum = iLinesPerPage * iPage + iLine;
                            if(iLineNum > iTotalLines) break;

                            //get line (iLine) from the application, and store it into szBuffer
							_mainWindow->getEditArea()->getToolkit()->getLine(iLineNum, szBuffer);

                            TextOut(deviceContext, 0, charHeight * iLine + iHeaderHeight, szBuffer,
                                    lstrlen(szBuffer) - 2);

                            bUserAbort = PrintProgressDialog::didUserCancelPrinting();
							delete [] szBuffer;
                        }

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

			PrintProgressDialog::hide();

        }
        else
        {
            bSuccess = FALSE;
        }

        if(bSuccess && !bUserAbort)
            EndDoc(deviceContext);
        else
            AbortDoc(deviceContext);

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
#define convert(x) ((int)(round(x * 100. / 254.)))

			rectangle.top    = convert(rectangle.top);
			rectangle.left   = convert(rectangle.left);
			rectangle.bottom = convert(rectangle.bottom);
			rectangle.right  = convert(rectangle.right);
		}
	}

	delete [] localeInfo;
}
