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


