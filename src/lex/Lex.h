
#ifndef LEX_H
#define LEX_H

#include <cstdint>

#define LEX_TYPE_INCLUDE			1
#define LEX_TYPE_GLOBAL_VARIABLE	2
#define LEX_TYPE_FUNCTION			3

class Lex
{
public:

	uint8_t type;
};

#endif
