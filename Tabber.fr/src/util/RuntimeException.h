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

#ifndef RUNTIMEEXCEPTION_H
#define RUNTIMEEXCEPTION_H

#include "../util/utilities.h"


class RuntimeException
{

	public:
	
		RuntimeException(const char* , const char* );
		RuntimeException(const char* , RuntimeException* );
		
		virtual ~RuntimeException();

		const char* getStackTrace() const;
		
	protected:

		void setStackTrace(const char* , const char* , bool );
	
	protected:
	
		char*             _stackTrace;
		RuntimeException* _sourceException;
};


#endif // RUNTIMEEXCEPTION_H

