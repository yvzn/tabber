#include "RuntimeException.h" 


CRuntimeException::CRuntimeException (
	LPCSTR szExecutionContext,
 	LPCSTR szErrorMessage )
{
	setStackTrace(szExecutionContext, szErrorMessage, FALSE);
	OBJECT_CREATED;
}


/*!
 * Used for higher-level exception propagation
 * @param lpSourceException the lower-level exception
 */
CRuntimeException::CRuntimeException (
	LPCSTR             szExecutionContext,
 	CRuntimeException* lpSourceException )
{
	setStackTrace(szExecutionContext, lpSourceException->getStackTrace(), TRUE);
	
	//okay boy, I've registered your information, you can gow now
	delete lpSourceException;

	OBJECT_CREATED;
}


CRuntimeException::~CRuntimeException()
{
	GlobalFree(m_szStackTrace);
	OBJECT_DELETED;
}


void CRuntimeException::setStackTrace(
	LPCSTR szExecutionContext,
 	LPCSTR szErrorMessage,
  	BOOL   bIsHigherLevel )
{
	UINT uLength;
	
	uLength = 6 + lstrlen(szExecutionContext) + lstrlen(szErrorMessage);
	if(!bIsHigherLevel)	uLength++;
	
	m_szStackTrace = (LPSTR)GlobalAlloc(GPTR, uLength);
		
	lstrcpy(m_szStackTrace, szErrorMessage);
	lstrcat(m_szStackTrace, "\n");
	if(!bIsHigherLevel)	lstrcat(m_szStackTrace, "\n");
	lstrcat(m_szStackTrace, "at ");
	lstrcat(m_szStackTrace, szExecutionContext);
	lstrcat(m_szStackTrace, "()");
}


LPCSTR CRuntimeException::getStackTrace()
{
	return m_szStackTrace;
}


