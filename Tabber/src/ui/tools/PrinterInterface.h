#ifndef PRINTERINTERFACE_H
#define PRINTERINTERFACE_H

#include "../../util/utilities.h"
#include "../../ui/dialogs/PrintProgressDialog.h"
#include "../../ui/tools/PrinterPager.h"

/* forward */ class MainWindow;


/**
 * Interface for printing jobs
 */
class PrinterInterface
{

	public:

		PrinterInterface(MainWindow* );
		~PrinterInterface();

		bool onChoosePageSetup();
		void onPrint();

	protected:

		void convertToThousandthsOfInches(RECT& );

	protected:

		MainWindow* _mainWindow;

		PAGESETUPDLG _pageSetupOptions;
		PRINTDLG     _printOptions;

		bool _arePageSetupOptionsValid;
};

#endif // PRINTERINTERFACE_H

