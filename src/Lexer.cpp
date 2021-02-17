
#include "Lexer.h"

#include <string>
#include <iostream>

const char* RESERVED_KEYWORDS[] = 
{
	"const", "class", "interface", "enum", "implements", "extends",
	"switch", "case", "if", "else", "int", "short", "byte", "long",
	"double", "float", "void", "unsigned", "goto", "break", "continue",
	"catch", "extern", "for", "while", "null", "true", "false", "delete",
	"using", "public", "private", "protected", "return", "sizeof",
	"new", "throw", "char", "wchar"
};

const char* RESERVED_PRIMITIVES[] = {
	"int", "short", "byte", "long", "double", "float", "bool"
};

const char* RESERVED_INTEGERS[] = {
	"int", "short", "byte", "long"
};

bool isReserved(const char** list, int listSize, const wchar_t* check)
{
	for(int i=0;i<listSize;i++)
	{
		const char* item = list[i];

		for(int j=0;;j++)
		{
			if(item[j] == 0 && check[j] == 0) {
				return true;
			}

			if(item[j] == 0 || check[j] == 0) {
				break;
			}

			if(item[j] != check[j]) {
				break;
			}
		}
	}

	return false;
}

Lexer::Lexer(std::wstring code)
{
	this->code = code;
}

void Lexer::process()
{
	StringReader reader(code.c_str(), code.length());
	
/*	try
	{*/
		while(!reader.atEnd())
		{
			reader.skipWhitespaces();
	
			// Import
			if(reader.skipIfNext(L"using"))
			{
				reader.skipWhitespaces();
				std::wstring package = L"";

				while(true)
				{
					wchar_t c = reader.getChar();

					package += reader.getKeyword();

					c = reader.getChar();

					if(c == '.')
					{
						package += c;
						reader.step();

						continue;
					}

					if(c == '*')
					{
						package += c;
						reader.step();
					}
	
					reader.skipWhitespaces();
					c = reader.getChar();

					if(c == ';') {
						reader.step();
						break;
					}
	
					throw CompileError(COMPILE_ERROR_SYNTAX, reader);
				}
	
				std::wcout << "Package name: " << package << "\n";
	
				continue;
			}

			// Anything else: variables, functions
			{
				int step = 0;
				bool isConst = false;
				bool isUnsigned = false;
				std::wstring type = L"", name = L"", mods=L"";

				// Get the flags and the last 2 keywords
				while(true)
				{
					wchar_t c = reader.getChar();
					
					if(step == 1)
					{
						if(c == '*') {
							mods += L"*";
							reader.step();
							reader.skipWhitespaces();
							continue;
						}
	
						if(c == '[')
						{
							mods += L"[";
							reader.step();
	
							while(c != ']') {
								c = reader.getChar();
								reader.step();
								mods += c;
							}
	
							reader.skipWhitespaces();
						}
					}

					std::wstring keyword = reader.getKeyword();
					reader.skipWhitespaces();
						
					if(keyword.length() == 0) {
						throw CompileError(COMPILE_ERROR_SYNTAX, reader);
					}

					if(step == 0)
					{
						if(keyword == L"const") {
							isConst = true;
							continue;
						}

						else if(keyword == L"unsigned") {
							isUnsigned = true;
							continue;
						}
					}

					if(step == 0) {
						type = keyword;
						step = 1;
					}

					else if(step == 1) {
						name = keyword;
						break;
					}
				}

				// Is the type not an integer and is unsigned checked
				if(!isReserved(RESERVED_INTEGERS, 4, type.c_str()) && isUnsigned) {
					throw CompileError(COMPILE_ERROR_RESERVED_KEYWORD, reader);
				}

				wchar_t c = reader.getChar();

				// Variable
				if(c == '=')
				{
					std::wcout << "Set (" << type << mods << ") " << name << " unsigned=" << isUnsigned << " const=" << isConst << "\n";

					reader.step();
					reader.skipWhitespaces();
					
					c = reader.getChar();

					if(c == ';') {
						reader.step();
					}
				}

				// Function
				else if(c == '(')
				{
					std::wcout << "Function (" << type << mods << ") " << name << " unsigned=" << isUnsigned << "\n";

					if(isConst) {
						throw CompileError(COMPILE_ERROR_RESERVED_KEYWORD, reader);
					}

					reader.step();
					reader.skipWhitespaces();
					
					c = reader.getChar();

					if(c == ';') {
						reader.step();
					}
				}
			}
		}
/*	}

	catch(CompileError &e)
	{
		std::cout << "caught exception\n";
	}*/
}
