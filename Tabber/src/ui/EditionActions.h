#ifndef EDITIONACTIONS_H
#define EDITIONACTIONS_H

#include "../util/utilities.h"
#include "../core/ApplicationSettings.h"
#include "../ui/EditionToolkit.h"



// ACTIONS ////////////////////////////////////////////////////////////////////

/**
 * A little hierarchy of objects used to perform edition actions (add chords, delete characters, ...) on staffs
 */
class StaffAction
{

	public:

		virtual ~StaffAction();

		/**
		 * Performs associated action, at the specified positon inside a staff
		 * @param toolkit a toolkit used for easier access to required information
		 * @param isHeader true if actions is performed as a header action (headers actions display chord names, titles, etc.)
		 */
		virtual void applyAt(unsigned int line, unsigned int column, EditionToolkit* toolkit, bool isHeader =false) =0;

		/**
		 * Returns the number of characters affected by operation, so that caret/cursor can be moved correctly
		 */
		inline virtual int getActionOffset() { return 0; }

};



// DISPATCHERS ////////////////////////////////////////////////////////////////


/**
 * Dispatchers are used to distribute chord notes on staffs, vertical bars, arpeggios and so on.
 */
class Dispatcher: public StaffAction
{

	public:
		
		virtual ~Dispatcher();

		virtual void applyAt(unsigned int , unsigned int , EditionToolkit* , bool );

		inline virtual int getActionOffset() { return getStringWidth(); }

	protected:

		       virtual const char* getString       (int ) const =0;
		inline virtual int         getStringWidth  ()     const { return 1;  };
		inline virtual const char* getHeaderString ()     const { return ""; };

};


/**
 * Always returns the same string, "|", which results in displaying a vertical bar at cursor position
 */
class BarDispatcher: public Dispatcher
{
	protected:

		inline const char* getString(int ) const { return "|"; }
};


/**
 * Always return a blank character, except for one line where it returs the string corresponding to specified key stroke
 */
class KeyStrokeDispatcher: public Dispatcher
{

	public:

		KeyStrokeDispatcher(int , int );
		~KeyStrokeDispatcher();

	protected:

		inline const char* getString(int l) const { return (l==_line) ? _string : ""; }

	protected:

		char* _string;
  		int _line;

};


/**
 * Distributes a chord.
 * Directly extends StaffAction because it is slightly more complex than other dispatchers.
 */
class ChordDispatcher: public StaffAction
{

	public:

		ChordDispatcher(GuitarChord* );

		virtual void applyAt(unsigned int , unsigned int , EditionToolkit* , bool );

		virtual int getActionOffset() { assert(_width >= 0); return _width; }

	protected:

		GuitarChord* _chord;
		int _width;

};


// REMOVERS ///////////////////////////////////////////////////////////////////


/**
 * Removers are used to remove characters at cursor position
 */
template <int direction>
class Remover: public StaffAction
{
	public:

		virtual void applyAt(unsigned int , unsigned int , EditionToolkit* , bool );
};


class BackwardRemover: public Remover<-1>
{
 	public:

		inline int getActionOffset() { return -1; }
};


class ForwardRemover: public Remover<+1>
{
 	public:

		// I do not need to set an offset because when you press Delete, the cursor does not move
		inline int getActionOffset() { return 0; }
};


// defined here because of template instantiation bugs
template <int direction>
void Remover<direction>::applyAt(unsigned int line, unsigned int column, EditionToolkit* toolkit, bool isHeader)
{
	unsigned int newColumn = column;
	unsigned int position = toolkit->getCharacterIndex(line, newColumn);

	if( column == newColumn
		&& (direction <=0
		|| (direction > 0 && column < toolkit->getLineLength(position))) )
	{
		toolkit->setSelection(position, position + direction);
		toolkit->replaceSelection("");
	}
}


#endif // EDITIONACTIONS_H

