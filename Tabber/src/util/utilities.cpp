#include "utilities.h"


/**
 * @throws a RuntimeException to abort the program "smoothly" if user decides so
 */
void __assertion_failed(const char* condition, const char* file, int line)
{
	if(NotifyMessage::assertionFailed(condition, file, line) != IDIGNORE )
	{
		throw new RuntimeException("::assert", "Assertion failed, program aborted by user");
	}
}


