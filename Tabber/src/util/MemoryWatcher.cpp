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
