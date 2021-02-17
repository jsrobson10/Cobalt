
#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <locale>

#include "Lexer.h"

int main(int argc, const char ** argv)
{
	if(argc != 2) {
		return 1;
	}

	const char* path = argv[1];

	std::ifstream file(path);
	std::string program_text_narrow;
	char c;

	while(file.get(c)) {
		program_text_narrow += c;
	}
	
	file.close();
	
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
	std::wstring program_text = convert.from_bytes(program_text_narrow);

	Lexer lexer(program_text);
	
	lexer.process();

	return 0;
}
