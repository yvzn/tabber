#include "EditionActions.h"
#include "../ui/EditArea.h"
#include "../ui/MainWindow.h"


// Actions ////////////////////////////////////////////////////////////////////

StaffAction::~StaffAction()
{ }



// Dispatcher /////////////////////////////////////////////////////////////////

Dispatcher::~Dispatcher()
{ }


void Dispatcher::applyAt(unsigned int line, unsigned int column, EditionToolkit* toolkit, bool isHeader)
{
	if(isHeader)
	{
		toolkit->copyAndFillAtLineCol(getHeaderString(), line, column, getStringWidth(), ' ');
	}
	else
	{
		toolkit->copyAndFillAtLineCol(getString(line), line, column, getStringWidth());
	}
}



// KeyStrokeDispatcher ////////////////////////////////////////////////////////

KeyStrokeDispatcher::KeyStrokeDispatcher(int virtualKeyCode, int line)
{
	_string = new char[2];
	lstrcpy(_string, (char*)&virtualKeyCode);
	_line = line;

	OBJECT_CREATED;
}


KeyStrokeDispatcher::~KeyStrokeDispatcher()
{
	delete [] _string;
	OBJECT_DELETED;
}



// ChordDispatcher ////////////////////////////////////////////////////////////

ChordDispatcher::ChordDispatcher(GuitarChord* chord)
{
	_chord = chord;
	_width = -1;
}


void ChordDispatcher::applyAt(unsigned int line, unsigned int column, EditionToolkit* toolkit, bool isHeader)
{
	static int lineFirstCall = line; // first call initialises this, so that I can easily fetch string # on next calls

	ApplicationSettings* settings =  toolkit->getEditArea()->getMainWindow()->getApplication()->getSettings(); 
	int  nameWidth  = lstrlen( _chord->getName() );
	if(_width < 0) // not initialised
	{
		_width = settings->isChordModeEnabled(ADD_NAME) ? max ( _chord->getWidth(), nameWidth ) : _chord->getWidth();
		if(settings->isChordModeEnabled(ADD_EXTRA_SPACE)) ++_width;
	}

	if(isHeader)
	{
		toolkit->copyAndFillAtLineCol(_chord->getName(), line, column, _width, ' ');
	}
	else
	{
		int string = line - lineFirstCall;
		if(string < _chord->getNoteCount())
		{		
			toolkit->copyAndFillAtLineCol(_chord->getNote( _chord->getNoteCount() - string - 1 ), line, column, _width);
		}
		else
		{
			toolkit->copyAndFillAtLineCol("", line, column, _width);
		}
	}
}

