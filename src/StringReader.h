
#ifndef STRING_READER_H
#define STRING_READER_H

#include <string>

const int STRING_READER_TYPE_UNKNOWN = 0;
const int STRING_READER_TYPE_KEYWORD = 1;
const int STRING_READER_TYPE_STRING = 2;
const int STRING_READER_TYPE_LSTRING = 3;
const int STRING_READER_TYPE_INTEGER = 4;
const int STRING_READER_TYPE_FLOATING = 5;
const int STRING_READER_TYPE_CHAR = 6;
const int STRING_READER_TYPE_LCHAR = 7;

class StringReader
{
private:
	const wchar_t *at;
	const wchar_t *start;
	const wchar_t *end;

public:
	StringReader(const wchar_t *code, int size);
	~StringReader();

	void skipWhitespaces();
	wchar_t getChar();
	wchar_t getChar(int offset);
	void step(int spaces);
	void step();
	bool atEnd();
	bool isNext(const wchar_t* check);
	bool skipIfNext(const wchar_t* check);
	bool isNext(std::wstring check);
	bool skipIfNext(std::wstring check);
	std::wstring getKeyword();
	std::wstring getString();
	std::wstring getLString();
	double getFloatingPoint();
	int getInteger();
	int getType();
};

#endif
