#ifndef DISPATCHERS_H
#define DISPATCHERS_H

#include "../util/utilities.h"


/**
 * A little hierarchy of objects used to distribute on staffs chord notes, vertical bars, arpeggios and so on.
 */
class Dispatcher
{

	public:
		
		virtual ~Dispatcher();

		virtual const char* getString       (int ) const =0;
		virtual int         getStringWidth  ()     const { return 1;  };
		virtual const char* getHeaderString ()     const { return ""; };

};


/**
 * Always returns the same string, "|", which results in displaying a vertical bar at cursor position
 */
class BarDispatcher: public Dispatcher
{
	
	public:

		inline const char* getString       (int ) const { return "|"; }

};


/**
 * Always return a blank character, except for one line where it returs the string corresponding to specified key stroke
 */
class KeyStrokeDispatcher: public Dispatcher
{

	public:

		KeyStrokeDispatcher(int , int );
		~KeyStrokeDispatcher();

		inline const char* getString       (int l) const { return (l==_line) ? _string : ""; }

	private:

		char* _string;
  		int _line;

};


#endif // DISPATCHERS_H

