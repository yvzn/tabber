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

#include "MemoryWatcher.h"

#include <list>
std::list<ALLOC_INFO*> *g_allocList;


MemoryWatcher::MemoryWatcher()
{
    isTrackingSafe = true;
	g_allocList = new std::list<ALLOC_INFO*>;
}


MemoryWatcher::~MemoryWatcher()
{
    isTrackingSafe = false; // no longer safe when memory watcher itself is being deleted
    delete g_allocList;
    g_allocList = NULL;
}


void MemoryWatcher::addTrack(DWORD address, DWORD size, const char *filename, DWORD line)
{
	ALLOC_INFO *info;

	if(isTrackingSafe)
	{
		info = new ALLOC_INFO;
		info->address = address;
		lstrcpy(info->file, filename);
		info->line = line;
		info->size = size;
		g_allocList->insert(g_allocList->begin(), info);   
	}	
}


void MemoryWatcher::removeTrack(DWORD address)
{
    std::list<ALLOC_INFO*>::iterator i;
    
    if(isTrackingSafe)
    {
		for(i = g_allocList->begin(); i != g_allocList->end(); i++)
 		{
			if((*i)->address == address)
			{
				g_allocList->remove(*i);
				break;
			}
		}
	}	
}


void MemoryWatcher::report() const
{
	std::list<ALLOC_INFO*>::iterator i;

	if(!g_allocList->empty())
	{
		DebugWindow::trace("Detected memory leaks:");

		for(i = g_allocList->begin(); i != g_allocList->end(); i++)
		{
			DebugWindow::trace(
  				"Memory at address %#x unfreed (%d bytes) (File: '%s', Line: %d)",
				(*i)->address, (*i)->size, (*i)->file, (*i)->line);
		}

//*
		int action = MessageBox(
			HWND_DESKTOP,
   			"Should I copy debugging trace content to clipboard ?",
  			"Tabber - Debug Trace",
   			MB_ICONQUESTION | MB_YESNO);

		if(action == IDYES)
		{
  			DebugWindow::copyTrace();
		}
//*/
	}
}
