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

