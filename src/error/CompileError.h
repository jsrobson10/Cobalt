
#ifndef COMPILE_ERROR_H
#define COMPILE_ERROR_H

#include "../StringReader.h"

const int COMPILE_ERROR_UNESCAPED_COMMENT = 1;
const int COMPILE_ERROR_SYNTAX = 2;
const int COMPILE_ERROR_RESERVED_KEYWORD = 3;

class CompileError
{
public:

	CompileError(int reason, StringReader reader);
};

#endif
