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

#include "PrinterPager.h"


PrinterPager::PrinterPager(EditArea* editArea, unsigned int linesPerPage)
{
	_maxLinesPerPage = linesPerPage;
	_toolkit = editArea->getToolkit();

	paginate();

	OBJECT_CREATED;
}


PrinterPager::~PrinterPager()
{
	OBJECT_DELETED;
}


/**
 * Computes and store the effective number of lines per page.
 * Tries to avoids page breaks inside of staffs and leaves at least one line
 * before each staff (for chords and stuff...)
 */
void PrinterPager::paginate()
{
	unsigned int lineCount = _toolkit->getLineCount();

	unsigned int currentPageStart = 0;
	unsigned int currentLine = 0;
	unsigned int linesOnCurrentPage = 1;

	unsigned int currentStaffStart = 0;
	bool isPreviousLineInsideStaff = false;

	while(currentLine < lineCount)
	{
		bool isInsideStaff = _toolkit->isStaffLine(currentLine);

		if(isInsideStaff && !isPreviousLineInsideStaff)
		{
			currentStaffStart = max(0, currentLine - 1);
		}

		if(linesOnCurrentPage >= _maxLinesPerPage)
		{
			addElement(new Integer(currentPageStart));

			if(isInsideStaff && currentLine - currentStaffStart < _maxLinesPerPage)
			{
				//inside staff && current staff fits completely on page
				currentPageStart = currentStaffStart;
				linesOnCurrentPage = currentLine - currentStaffStart;
			}
			else
			{
				currentPageStart = currentLine;
				linesOnCurrentPage = 0;
			}
		}

		++linesOnCurrentPage;
		++currentLine;
		isPreviousLineInsideStaff = isInsideStaff;
	}

	//last value is not handled in the loop
	addElement(new Integer(currentPageStart));
}



int PrinterPager::getPageLineCount(int page)
{
	if(page < getPageCount() - 1)
	{
		return getPageFirstLine(page+1) - getPageFirstLine(page);
	}
	else
	{
		return _toolkit->getLineCount() - getPageFirstLine(page);
	}
}



