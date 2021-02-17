
#ifndef LEX_INCLUDE_H
#define LEX_INCLUDE_H

#include "Lex.h"

#include <string>

class LexInclude : public Lex
{
public:

	std::string path;

	LexInclude();

	void setPath(std::string path);
};

#endif
