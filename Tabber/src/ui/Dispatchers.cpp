#include "Dispatchers.h"


// Dispatcher /////////////////////////////////////////////////////////////////

Dispatcher::~Dispatcher()
{ }


// KeyStrokeDispatcher ////////////////////////////////////////////////////////


KeyStrokeDispatcher::KeyStrokeDispatcher(int virtualKeyCode, int line)
{
	_string = new char[2];
	lstrcpy(_string, (char*)&virtualKeyCode);
	_line = line;
}


KeyStrokeDispatcher::~KeyStrokeDispatcher()
{
	delete [] _string;
}
