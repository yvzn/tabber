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

