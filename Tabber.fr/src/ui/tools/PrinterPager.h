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

#ifndef PRINTERPAGER_H
#define PRINTERPAGER_H

#include "../../util/utilities.h"
#include "../../ui/EditArea.h"
#include "../../ui/tools/EditionToolkit.h"

//int wrapper
class Integer
{
	public:

		inline Integer(int i) { _value=i;      }
		inline int getValue() { return _value; }

	protected:

		int _value;
};



/**
 * Paginates document to be printed
 */
class PrinterPager: protected DynamicPointerArray<Integer>
{

	public:

		PrinterPager(EditArea* , unsigned int );
		~PrinterPager();

		inline int getPageFirstLine(int page) { return getElementAt(page)->getValue(); }
		       int getPageLineCount(int page);
		inline int getPageCount()             { return getElementCount();  }

	protected:

		void paginate();

	protected:

		unsigned int    _maxLinesPerPage;
		EditionToolkit* _toolkit;

};


#endif // PRINTERPAGER_H

