#ifndef DOUBLENULLSTRINGTOKENIZER_H
#define DOUBLENULLSTRINGTOKENIZER_H

#include "../util/utilities.h"


class DoubleNullStringTokenizer
{

	public:

		DoubleNullStringTokenizer(const char* , int );
		~DoubleNullStringTokenizer();

		bool        hasMoreTokens() const;
		const char* nextToken();
		
	protected:
	
		const char* _string;
		char* _endOfString;
		char* _currentToken;
};

#endif // DOUBLENULLSTRINGTOKENIZER_H

