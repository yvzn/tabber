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

#ifndef EDITIONACTIONS_H
#define EDITIONACTIONS_H

#include "../../util/utilities.h"
#include "../../core/ApplicationSettings.h"
#include "../../ui/tools/EditionToolkit.h"



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

		       virtual const char* getString       (int ) =0;
		inline virtual int         getStringWidth  ()     { return 1;  };
		inline virtual const char* getHeaderString ()     { return ""; };

};


/**
 * Always returns the same string, "|", which results in displaying a vertical bar at cursor position
 */
class BarDispatcher: public Dispatcher
{
	protected:

		inline const char* getString(int ) { return "|"; }
};


/**
 * Always return a blank character, except for one line where it returs the string corresponding to specified key stroke
 */
class SingleNoteDispatcher: public Dispatcher
{

	public:

		SingleNoteDispatcher(const char* , int );
		SingleNoteDispatcher(int , int );
		~SingleNoteDispatcher();

	protected:

		inline const char* getString  (int l)  { return (l==_line) ? _string : ""; }
		inline int         getStringWidth  ()  { return _width;  };
		inline const char* getHeaderString ()  { return (_line < 0) ? _string : ""; };

	protected:

		char* _string;
		int _width;
  		int _line;

};


/**
 * Distributes a multiple-note object(chord or tuning)
 */
class MultipleNoteDispatcher: public Dispatcher
{

	public:

		MultipleNoteDispatcher(GuitarChord* );
		virtual ~MultipleNoteDispatcher();

		virtual void applyAt(unsigned int , unsigned int , EditionToolkit* , bool );

	protected:

		       virtual const char* getString   (int );
		inline virtual int         getStringWidth  () { assert(_width >= 0); return _width; }
		       virtual const char* getHeaderString () =0;
		       virtual const char* getNote     (int ) =0;
		       virtual void initStringWidth (EditionToolkit* ) =0;

	protected:

		GuitarChord* _chord;
		int _width;
		int _lineFirstCall;

};


class ChordDispatcher: public MultipleNoteDispatcher
{

	public:

		ChordDispatcher(GuitarChord* );

	protected:

		inline const char* getHeaderString () { return _chord->getName();  }
		inline const char* getNote     (int );
		       void initStringWidth (EditionToolkit* );

};


class TuningDispatcher: public MultipleNoteDispatcher
{

	public:

		TuningDispatcher(GuitarTuning* );
		~TuningDispatcher();

		void applyAt(unsigned int , unsigned int , EditionToolkit* , bool );

	protected:

		inline const char* getHeaderString () { return ""; }
		       const char* getNote    (int n);
		inline void initStringWidth (EditionToolkit* ) { _width = _chord->getWidth()+1; }

	protected:

		char* _buffer; // I use a buffer to append a separator '|' after tuning notes

};


class ArpeggioDispatcher: public MultipleNoteDispatcher
{

	public:

		typedef enum { NONE=0, ASCENDING, DESCENDING, DIRECTION_COUNT } Direction;

		ArpeggioDispatcher(GuitarChord* , Direction );
		~ArpeggioDispatcher();

	protected:

		inline const char* getHeaderString () { return _chord->getName(); }
		       const char* getNote    (int n);
		       void initStringWidth (EditionToolkit* );

	protected:

		char*     _buffer; // I use a buffer to construct content
		bool      _addSpace;
		Direction _direction;

};


// REMOVERS ///////////////////////////////////////////////////////////////////


/**
 * Removers are used to remove characters at cursor position
 */
class Remover: public StaffAction
{
	public:

		inline Remover(int dir) : _direction(dir) { }

		virtual void applyAt(unsigned int , unsigned int , EditionToolkit* , bool );

	protected:
		
		int _direction;
};


class BackwardRemover: public Remover
{
 	public:

		inline BackwardRemover() : Remover(-1) { }

		inline int getActionOffset() { return -1; }
};


class ForwardRemover: public Remover
{
 	public:

		inline ForwardRemover() : Remover(+1) { }

		// I do not need to set an offset because when you press Delete, the cursor does not move
		inline int getActionOffset() { return 0; }
};


#endif // EDITIONACTIONS_H

