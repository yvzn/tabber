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

#ifndef MEMORYWATCHER_H
#define MEMORYWATCHER_H

#include <windows.h>

#include "../util/DebugWindow.h"


typedef struct
{
	DWORD	address;
	DWORD	size;
	char	file[64];
	DWORD	line;
} ALLOC_INFO;


/**
 * A tool to trace memory allocations and detect leaks
 * @see http://www.flipcode.com/tutorials/tut_memleak.shtml
 */
class MemoryWatcher
{

	public:

		MemoryWatcher();
		~MemoryWatcher();

		void addTrack(DWORD, DWORD, const char*, DWORD );
		void removeTrack(DWORD );
		void report() const;

	private:

		bool isTrackingSafe; //< Memory tracking might not safe at certain points of the program. In those cases, tracking can be disabled by hand.
};


#endif // MEMORYWATCHER_H

