
#include "LexInclude.h"

LexInclude::LexInclude() {
	this->type = LEX_TYPE_INCLUDE;
}

void LexInclude::setPath(std::string path) {
	this->path = path;
}
