
#include <cstring>
#include <string>
#include <stdexcept>

#include "StringReader.h"
#include "error/CompileError.h"

StringReader::StringReader(const wchar_t* code, int size)
{
	start = code;
	end = start + size;
	at = code;
}

StringReader::~StringReader()
{

}

wchar_t StringReader::getChar()
{
	if(at < end) {
		return *at;
	}

	return 0;
}

wchar_t StringReader::getChar(int offset)
{
	const wchar_t* c = at + offset;

	if(c < end) {
		return *c;
	}

	return 0;
}

bool StringReader::atEnd() {
	return at == end;
}

void StringReader::step(int spaces)
{
	at += spaces;

	if(at > end) {
		at = end;
	}
}

void StringReader::step()
{
	at += 1;

	if(at > end) {
		at = end;
	}
}

void StringReader::skipWhitespaces()
{
	// 0 = no comments, 1 = single line comment, 2 = multi line comment
	int mode = 0;

	while(true)
	{
		wchar_t next = getChar();

		if(next == 0)
		{
			if(mode == 2) {
				throw CompileError(COMPILE_ERROR_UNESCAPED_COMMENT, *this);
			}

			step();
			return;
		}

		// End of single line comment
		if(next == '\n' && mode == 1) {
			mode = 0;
			step();
			continue;
		}

		// End of multi line comment
		if(mode == 2 && next == '*' && getChar(1) == '/') {
			mode = 0;
			step(2);
			continue;
		}

		if(mode != 0) {
			step();
			continue;
		}

		// Possibly the start of a comment
		if(next == '/')
		{
			wchar_t next2 = getChar(1);

			// Single line comment
			if(next2 == '/') {
				mode = 1;
				step(2);
				continue;
			}

			// Multi line comment
			if(next2 == '*') {
				mode = 2;
				step(2);
				continue;
			}
		}

		if(next == ' ' || next == '\t' || next == '\n' || next == '\r') {
			step();
			continue;
		}

		return;
	}
}

bool StringReader::isNext(const wchar_t* check)
{
	for(int i=0;;i++)
	{
		const wchar_t c1 = getChar(i);
		const wchar_t c2 = check[i];

		if(c2 == 0) {
			return true;
		}

		if(c1 != c2) {
			return false;
		}
	}
}

bool StringReader::skipIfNext(const wchar_t* check)
{
	for(int i=0;;i++)
	{
		const wchar_t c1 = getChar(i);
		const wchar_t c2 = check[i];

		if(c2 == 0)
		{
			step(i);
			
			return true;
		}

		if(c1 != c2) {
			return false;
		}
	}
}

bool StringReader::isNext(std::wstring check) {
	return isNext(check.c_str());
}

bool StringReader::skipIfNext(std::wstring check) {
	return skipIfNext(check.c_str());
}

std::wstring StringReader::getKeyword()
{
	std::wstring keyword = L"";

	while(true)
	{
		wchar_t c = getChar();

		if(c < '0' || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= 127)) {
			return keyword;
		}

		keyword += c;
		step();
	}
}

int StringReader::getType()
{
	wchar_t c = getChar();

	// Could be a number
	if((c >= '0' && c <= '9') || c == '-' || c == '+')
	{
		bool atStart = (c >= '0' && c <= '9');
		bool afterE = false;
		
		std::string num = "" + (char)c;

		StringReader reader = *this;
		reader.step();

		while(true)
		{
			c = getChar();
			step();

			if(c >= '0' && c <= '9') {
				atStart = true;
				afterE = false;
				num += (char)c;
				continue;
			}

			if(c == 'e' || c == 'E') {
				afterE = true;
				num += (char)c;
				continue;
			}

			if((c == '+' || c == '-') && (atStart || afterE)) {
				num += (char)c;
				continue;
			}

			if(c == '.') {
				num += (char)c;
				continue;
			}
			
			break;
		}

		try {
			std::stol(num);
			return STRING_READER_TYPE_INTEGER;
		}

		catch(std::out_of_range &e) {
			return STRING_READER_TYPE_FLOATING;
		}
	}

	if(c == '"') {
		return STRING_READER_TYPE_STRING;
	}

	if(c == '\'') {
		return STRING_READER_TYPE_CHAR;
	}

	if(c == 'L')
	{
		wchar_t c2 = getChar(1);
		
		if(c2 == '"') {
			return STRING_READER_TYPE_LSTRING;
		}

		if(c2 == '\'') {
			return STRING_READER_TYPE_LCHAR;
		}
	}

	return STRING_READER_TYPE_KEYWORD;
}
