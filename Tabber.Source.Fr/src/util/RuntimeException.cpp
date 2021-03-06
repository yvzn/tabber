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

#include "RuntimeException.h" 


RuntimeException::RuntimeException (
	const char* executionContext,
 	const char* errorMessage )
{
	setStackTrace(executionContext, errorMessage, false);
	OBJECT_CREATED;
}


/*!
 * Used for higher-level exception propagation
 * @param sourceException the lower-level exception
 */
RuntimeException::RuntimeException (
	const char*        executionContext,
	RuntimeException*  sourceException )
{
	setStackTrace(executionContext, sourceException->getStackTrace(), true);

	//okay boy, I've registered your information, you can gow now
	delete sourceException;

	OBJECT_CREATED;
}


RuntimeException::~RuntimeException()
{
	delete [] _stackTrace;
	OBJECT_DELETED;
}


void RuntimeException::setStackTrace(
	const char* executionContext,
 	const char* errorMessage,
  	bool        isHigherLevel )
{
	UINT stackLength = 7 + lstrlen(executionContext) + lstrlen(errorMessage);
	if(!isHigherLevel)	stackLength++;

	_stackTrace = new char[stackLength+1];

	lstrcpy(_stackTrace, errorMessage);
	lstrcat(_stackTrace, "\n");
	if(!isHigherLevel)	lstrcat(_stackTrace, "\n");
	lstrcat(_stackTrace, "at ");
	lstrcat(_stackTrace, executionContext);
	lstrcat(_stackTrace, "()");
}


const char* RuntimeException::getStackTrace() const
{
	assert(_stackTrace != NULL);
	return _stackTrace;
}


