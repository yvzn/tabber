#ifndef RUNTIMEEXCEPTION_H
#define RUNTIMEEXCEPTION_H

#include "../core/utilities.h"


class CRuntimeException
{

	public:
	
		CRuntimeException(LPCSTR , LPCSTR );
		CRuntimeException(LPCSTR , CRuntimeException* );
		
		virtual ~CRuntimeException();

		LPCSTR getStackTrace();
		
	protected:

		void setStackTrace(LPCSTR , LPCSTR , BOOL );
	
	protected:
	
		LPSTR              m_szStackTrace;
		CRuntimeException* m_lpSourceException;
};


#endif // RUNTIMEEXCEPTION_H

